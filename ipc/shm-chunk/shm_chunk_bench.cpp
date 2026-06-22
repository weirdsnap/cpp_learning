// chunk + ring 共享内存端到端性能基准测试
// 测量 100MB 数据流从 Producer 写入到 Consumer 读取完成的总吞吐

#include "shm_chunk.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <emmintrin.h>
#include <iostream>

inline constexpr size_t BENCH_STREAM_SIZE = 1000ULL * 1024 * 1024;  // 1000MB

using ns = std::chrono::nanoseconds;
using hr_clock = std::chrono::high_resolution_clock;

static uint64_t now_ns() {
    return std::chrono::duration_cast<ns>(hr_clock::now().time_since_epoch()).count();
}

static void producer(ShmLayout layout) {
    size_t offset = 0;
    uint32_t seq = 0;

    while (offset < BENCH_STREAM_SIZE) {
        // 等待空闲 chunk
        uint32_t a = layout.header->chunk_alloc_idx.load(std::memory_order_relaxed);
        uint32_t r = layout.header->chunk_return_idx.load(std::memory_order_acquire);
        while (a == r) {  // free ring 空时等待
            _mm_pause();
            r = layout.header->chunk_return_idx.load(std::memory_order_acquire);
        }
        uint32_t cid = layout.free_array[a % CHUNK_COUNT];
        layout.header->chunk_alloc_idx.store(a + 1, std::memory_order_release);

        size_t this_len = std::min(CHUNK_SIZE, BENCH_STREAM_SIZE - offset);

        // 填充数据：前 4 字节为序号，其余为递增字节模式
        uint8_t* dst = reinterpret_cast<uint8_t*>(layout.chunk_pool[cid].data);
        std::memcpy(dst, &seq, sizeof(seq));
        for (size_t i = sizeof(seq); i < this_len; ++i) {
            dst[i] = static_cast<uint8_t>((offset + i) & 0xFFu);
        }

        // 等待 desc ring 空间
        uint64_t dw = layout.header->desc_write_idx.load(std::memory_order_relaxed);
        uint64_t dr = layout.header->desc_read_idx.load(std::memory_order_acquire);
        while (dw - dr >= DESC_COUNT - 1) {
            _mm_pause();
            dr = layout.header->desc_read_idx.load(std::memory_order_acquire);
        }

        ChunkDesc desc{};
        desc.chunk_id = cid;
        desc.payload_len = static_cast<uint32_t>(this_len);
        desc.timestamp_ns = now_ns();
        desc.crc32 = crc32(layout.chunk_pool[cid].data, this_len);
        desc.flags = (offset + this_len >= BENCH_STREAM_SIZE) ? 1u : 0u;
        layout.desc_ring[dw % DESC_COUNT] = desc;
        layout.header->desc_write_idx.store(dw + 1, std::memory_order_release);

        offset += this_len;
        ++seq;
    }
}

static int consumer(ShmLayout layout) {
    size_t total_bytes = 0;
    uint32_t expected_seq = 0;
    uint32_t errors = 0;
    bool finished = false;

    while (!finished) {
        uint64_t dr = layout.header->desc_read_idx.load(std::memory_order_relaxed);
        uint64_t dw = layout.header->desc_write_idx.load(std::memory_order_acquire);
        while (dr == dw) {
            _mm_pause();
            dw = layout.header->desc_write_idx.load(std::memory_order_acquire);
        }

        const ChunkDesc& desc = layout.desc_ring[dr % DESC_COUNT];
        const Chunk& chunk = layout.chunk_pool[desc.chunk_id];
        const uint8_t* src = reinterpret_cast<const uint8_t*>(chunk.data);

        uint32_t actual_seq = 0;
        std::memcpy(&actual_seq, src, sizeof(actual_seq));
        if (actual_seq != expected_seq) {
            ++errors;
            expected_seq = actual_seq;
        }

        uint32_t actual_crc = crc32(chunk.data, desc.payload_len);
        if (actual_crc != desc.crc32) {
            ++errors;
        }

        // 归还 chunk
        uint32_t r = layout.header->chunk_return_idx.load(std::memory_order_relaxed);
        uint32_t a = layout.header->chunk_alloc_idx.load(std::memory_order_acquire);
        while (r - a >= CHUNK_COUNT) {  // free ring 满时等待
            _mm_pause();
            a = layout.header->chunk_alloc_idx.load(std::memory_order_acquire);
        }
        layout.free_array[r % CHUNK_COUNT] = desc.chunk_id;
        layout.header->chunk_return_idx.store(r + 1, std::memory_order_release);

        layout.header->desc_read_idx.store(dr + 1, std::memory_order_release);

        total_bytes += desc.payload_len;
        ++expected_seq;

        if (desc.flags & 1u) finished = true;
    }

    if (errors != 0) {
        std::cerr << "[Consumer] CRC/序列错误数: " << errors << "\n";
        return 1;
    }
    if (total_bytes != BENCH_STREAM_SIZE) {
        std::cerr << "[Consumer] 数据量不匹配: " << total_bytes << "\n";
        return 1;
    }
    return 0;
}

int main() {
    // 清理旧共享内存
    shm_unlink(SHM_CHUNK_NAME);

    size_t sz = shm_total_size();
    std::cout << "[Bench] 共享内存: " << sz / (1024 * 1024) << " MB\n";
    std::cout << "[Bench] 测试流量: " << BENCH_STREAM_SIZE / (1024 * 1024) << " MB\n";

    int fd = shm_open(SHM_CHUNK_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }
    if (ftruncate(fd, sz) == -1) { perror("ftruncate"); return 1; }

    void* ptr = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return 1; }
    close(fd);

    ShmLayout layout = parse_shm(ptr);
    init_shm(layout);

    pid_t pid = fork();
    if (pid == -1) { perror("fork"); return 1; }

    uint64_t start_ns = now_ns();

    if (pid == 0) {
        // 子进程：Producer
        producer(layout);
        munmap(ptr, sz);
        _exit(0);
    }

    // 父进程：Consumer
    int ret = consumer(layout);
    uint64_t end_ns = now_ns();

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "[Bench] Producer 异常退出\n";
        ret = 1;
    }

    if (ret == 0) {
        double sec = (end_ns - start_ns) / 1e9;
        double mb = BENCH_STREAM_SIZE / (1024.0 * 1024.0);
        double mbps = mb / sec;
        double gbps = mbps * 8 / 1024;

        std::cout << "[Bench] 总时间: " << sec << " s\n";
        std::cout << "[Bench] 吞吐: " << mbps << " MB/s (" << gbps << " Gb/s)\n";
    }

    munmap(ptr, sz);
    shm_unlink(SHM_CHUNK_NAME);
    return ret;
}
