// chunk + ring 结合的共享内存消费者
// 关键点：
// 1. 从 desc_ring 读取描述符
// 2. 直接读 chunk_pool[desc.chunk_id].data，零拷贝
// 3. 独立计算 CRC 并与描述符中的 crc32 对比
// 4. 归还 chunk_id 到 free_array 供 Producer 复用

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
#include <emmintrin.h>

int main(int argc, char* argv[]) {
    int stream_count = (argc > 1) ? std::atoi(argv[1]) : 1;
    bool test_mode = (argc > 1);

    int fd = shm_open(SHM_CHUNK_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    size_t shm_size = shm_total_size();
    void* addr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    ShmLayout layout = parse_shm(addr);

    std::cout << "[Consumer] 开始消费，共 " << stream_count << " 次流...\n";

    size_t total_bytes = 0;
    uint32_t total_chunks = 0;
    uint32_t errors = 0;

    for (int s = 0; s < stream_count; ++s) {
        bool stream_finished = false;
        uint32_t expected_seq = 0;

        while (!stream_finished) {
            ChunkDesc desc{};
            // 忙等或轮询直到有数据
            while (!pop_desc(layout, desc)) {
                _mm_pause();
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }

            // 零拷贝：直接读共享内存中的 chunk 数据
            const Chunk& chunk = layout.chunk_pool[desc.chunk_id];
            const uint8_t* src = reinterpret_cast<const uint8_t*>(chunk.data);

            // 校验序号
            uint32_t actual_seq = 0;
            std::memcpy(&actual_seq, src, sizeof(actual_seq));
            if (actual_seq != expected_seq) {
                std::cerr << "[Consumer] 序号错误! 期望 " << expected_seq
                          << " 实际 " << actual_seq << "\n";
                ++errors;
                expected_seq = actual_seq;
            }

            // 校验 CRC
            uint32_t actual_crc = crc32(chunk.data, desc.payload_len);
            if (actual_crc != desc.crc32) {
                std::cerr << "[Consumer] CRC 错误! seq=" << expected_seq << "\n";
                ++errors;
            }

            // 归还 Chunk，供 Producer 复用
            return_chunk(layout, desc.chunk_id);

            total_bytes += desc.payload_len;
            ++total_chunks;
            ++expected_seq;

            if (desc.flags & 1u) {
                stream_finished = true;
            }
        }

        if (!test_mode) {
            std::cout << "[Consumer] 完成第 " << s << " 次流，"
                      << total_chunks << " 个 Chunk，"
                      << (total_bytes / (1024 * 1024)) << " MB\n";
        }
    }

    if (errors == 0) {
        std::cout << "[Consumer] 全部校验通过，共 " << total_chunks
                  << " 个 Chunk，" << (total_bytes / (1024 * 1024)) << " MB\n";
    } else {
        std::cout << "[Consumer] 错误数: " << errors << "\n";
        return 1;
    }

    munmap(addr, shm_size);
    return 0;
}
