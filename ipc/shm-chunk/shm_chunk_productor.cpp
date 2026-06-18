// chunk + ring 结合的共享内存生产者
// 关键点：
// 1. 100MB 数据流拆成多个 Chunk
// 2. 每个 Chunk 计算 CRC32
// 3. 通过 desc_ring 传递元数据
// 4. 最后一块标记 flags.bit0 = 1

#include "shm_chunk.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

inline constexpr size_t STREAM_SIZE = 100ULL * 1024 * 1024;  // 100MB

int main(int argc, char* argv[]) {
    int stream_count = (argc > 1) ? std::atoi(argv[1]) : 1;
    bool test_mode = (argc > 1);

    int fd = shm_open(SHM_CHUNK_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    size_t shm_size = shm_total_size();
    if (ftruncate(fd, shm_size) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    void* addr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    ShmLayout layout = parse_shm(addr);

    // 通过关键索引判断是否为首次创建
    if (layout.header->desc_write_idx.load(std::memory_order_relaxed) == 0 &&
        layout.header->chunk_return_idx.load(std::memory_order_relaxed) == 0) {
        std::cout << "[Producer] 初始化共享内存...\n";
        init_shm(layout);
    }

    std::cout << "[Producer] 开始生产，每次 " << (STREAM_SIZE / (1024 * 1024))
              << " MB，共 " << stream_count << " 次...\n";

    auto now = std::chrono::steady_clock::now;
    for (int s = 0; s < stream_count; ++s) {
        size_t offset = 0;
        uint32_t seq = 0;

        while (offset < STREAM_SIZE) {
            uint32_t chunk_id = alloc_chunk(layout);
            Chunk& chunk = layout.chunk_pool[chunk_id];

            size_t remaining = STREAM_SIZE - offset;
            size_t this_len = std::min(CHUNK_SIZE, remaining);

            // 构造可校验的载荷：前 4 字节为序号，后续填充递增字节
            uint8_t* dst = reinterpret_cast<uint8_t*>(chunk.data);
            std::memcpy(dst, &seq, sizeof(seq));
            for (size_t i = sizeof(seq); i < this_len; ++i) {
                dst[i] = static_cast<uint8_t>((offset + i) & 0xFFu);
            }

            ChunkDesc desc{};
            desc.chunk_id = chunk_id;
            desc.payload_len = static_cast<uint32_t>(this_len);
            desc.timestamp_ns = static_cast<uint64_t>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now().time_since_epoch()).count());
            desc.crc32 = crc32(chunk.data, this_len);
            desc.flags = (offset + this_len >= STREAM_SIZE) ? 1u : 0u;

            push_desc(layout, desc);

            offset += this_len;
            ++seq;
        }

        if (!test_mode) {
            std::cout << "[Producer] 完成第 " << s << " 次流，共 " << seq << " 个 Chunk\n";
        }
    }

    std::cout << "[Producer] 全部完成\n";

    if (!test_mode) {
        std::cout << "[Producer] 按回车键退出并清理...\n";
        std::cin.get();
        shm_unlink(SHM_CHUNK_NAME);
    }

    munmap(addr, shm_size);
    return 0;
}
