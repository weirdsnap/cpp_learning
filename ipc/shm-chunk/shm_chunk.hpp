#pragma once
#ifndef SHM_CHUNK_H
#define SHM_CHUNK_H

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <new>
#include <thread>
#include <chrono>

inline uint32_t crc32(const void* data, size_t len) {
    static uint32_t table[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int j = 0; j < 8; ++j)
                c = (c >> 1) ^ (0xEDB88320u & -(c & 1u));
            table[i] = c;
        }
        init = true;
    }
    const uint8_t* p = static_cast<const uint8_t*>(data);
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i)
        crc = table[(crc ^ p[i]) & 0xFFu] ^ (crc >> 8);
    return ~crc;
}

inline constexpr size_t CHUNK_SIZE    = 16ULL * 1024 * 1024;  // 16MB
inline constexpr uint32_t CHUNK_COUNT = 16;
inline constexpr uint32_t DESC_COUNT  = 64;
inline constexpr size_t PAGE_SIZE     = 4096;
inline constexpr const char* SHM_CHUNK_NAME = "/my_shm_chunk";

struct ChunkDesc {
    uint32_t chunk_id;        // 指向 Chunk Pool 的编号
    uint32_t payload_len;     // 实际用了多少字节
    uint64_t timestamp_ns;    // 生产者时间戳
    uint32_t crc32;           // 数据校验
    uint16_t flags;           // bit0: 1=最后一块
    uint16_t reserved;        // 填充
};
static_assert(sizeof(ChunkDesc) == 24);

struct alignas(4096) Chunk {
    char data[CHUNK_SIZE];
};

struct alignas(64) ShmHeader {
    std::atomic<uint64_t> desc_write_idx{0};
    char pad1[56];
    std::atomic<uint64_t> desc_read_idx{0};
    char pad2[56];
    std::atomic<uint32_t> chunk_alloc_idx{0};   // 下一个可分配的 chunk id 槽位
    char pad3[60];
    std::atomic<uint32_t> chunk_return_idx{0};  // 下一个归还 chunk id 的槽位
    char pad4[60];
    uint32_t chunk_count{CHUNK_COUNT};
    uint32_t chunk_size{static_cast<uint32_t>(CHUNK_SIZE)};
    uint32_t desc_count{DESC_COUNT};
    uint32_t reserved;
};

inline constexpr size_t align_up(size_t sz, size_t align = PAGE_SIZE) {
    return (sz + align - 1) & ~(align - 1);
}

inline constexpr size_t shm_total_size() {
    return align_up(sizeof(ShmHeader))
         + align_up(DESC_COUNT * sizeof(ChunkDesc))
         + align_up(CHUNK_COUNT * sizeof(uint32_t))
         + CHUNK_COUNT * CHUNK_SIZE;
}

struct ShmLayout {
    ShmHeader*   header;
    ChunkDesc*   desc_ring;
    uint32_t*    free_array;
    Chunk*       chunk_pool;
};

inline ShmLayout parse_shm(void* base) {
    char* p = static_cast<char*>(base);
    ShmLayout layout;
    layout.header = reinterpret_cast<ShmHeader*>(p);
    p += align_up(sizeof(ShmHeader));
    layout.desc_ring = reinterpret_cast<ChunkDesc*>(p);
    p += align_up(DESC_COUNT * sizeof(ChunkDesc));
    layout.free_array = reinterpret_cast<uint32_t*>(p);
    p += align_up(CHUNK_COUNT * sizeof(uint32_t));
    layout.chunk_pool = reinterpret_cast<Chunk*>(p);
    return layout;
}

inline void init_shm(const ShmLayout& layout) {
    // 用 placement new 重新构造 header，安全清零含 atomic 的结构
    new (layout.header) ShmHeader();
    layout.header->chunk_count = CHUNK_COUNT;
    layout.header->chunk_size = static_cast<uint32_t>(CHUNK_SIZE);
    layout.header->desc_count = DESC_COUNT;

    // 把所有 chunk id 放入空闲环
    for (uint32_t i = 0; i < CHUNK_COUNT; ++i) {
        layout.free_array[i] = i;
    }
    layout.header->chunk_return_idx.store(CHUNK_COUNT, std::memory_order_release);
    layout.header->chunk_alloc_idx.store(0, std::memory_order_release);
}

inline uint32_t alloc_chunk(const ShmLayout& layout) {
    auto& header = *layout.header;
    while (true) {
        uint32_t alloc = header.chunk_alloc_idx.load(std::memory_order_relaxed);
        uint32_t ret = header.chunk_return_idx.load(std::memory_order_acquire);
        if (alloc != ret) {
            uint32_t chunk_id = layout.free_array[alloc % CHUNK_COUNT];
            header.chunk_alloc_idx.store(alloc + 1, std::memory_order_release);
            return chunk_id;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

inline void return_chunk(const ShmLayout& layout, uint32_t chunk_id) {
    auto& header = *layout.header;
    while (true) {
        uint32_t ret = header.chunk_return_idx.load(std::memory_order_relaxed);
        uint32_t alloc = header.chunk_alloc_idx.load(std::memory_order_acquire);
        if (ret - alloc < CHUNK_COUNT) {
            layout.free_array[ret % CHUNK_COUNT] = chunk_id;
            header.chunk_return_idx.store(ret + 1, std::memory_order_release);
            return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

inline void push_desc(const ShmLayout& layout, const ChunkDesc& desc) {
    auto& header = *layout.header;
    while (true) {
        uint64_t write = header.desc_write_idx.load(std::memory_order_relaxed);
        uint64_t read = header.desc_read_idx.load(std::memory_order_acquire);
        if (write - read < DESC_COUNT) {
            layout.desc_ring[write % DESC_COUNT] = desc;
            header.desc_write_idx.store(write + 1, std::memory_order_release);
            return;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

inline bool pop_desc(const ShmLayout& layout, ChunkDesc& desc) {
    auto& header = *layout.header;
    while (true) {
        uint64_t read = header.desc_read_idx.load(std::memory_order_relaxed);
        uint64_t write = header.desc_write_idx.load(std::memory_order_acquire);
        if (read == write) {
            return false;  // 当前无数据，调用方可以选择等待或重试
        }
        desc = layout.desc_ring[read % DESC_COUNT];
        header.desc_read_idx.store(read + 1, std::memory_order_release);
        return true;
    }
}

#endif // SHM_CHUNK_H
