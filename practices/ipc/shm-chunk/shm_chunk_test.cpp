#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

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
#include <thread>

inline constexpr size_t TEST_STREAM_SIZE = 100ULL * 1024 * 1024;  // 100MB

static bool shm_exists(const char* name) {
    int fd = shm_open(name, O_RDONLY, 0666);
    if (fd != -1) { close(fd); return true; }
    return false;
}

static void wait_for_shm(const char* name, int retries = 200) {
    for (int i = 0; i < retries; ++i) {
        if (shm_exists(name)) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

TEST_CASE("chunk + ring 100MB 端到端：拆块、CRC、零拷贝、复用") {
    const size_t shm_size = shm_total_size();

    // 清理可能残留的旧共享内存
    shm_unlink(SHM_CHUNK_NAME);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        REQUIRE(false);
    }

    if (pid == 0) {
        // 子进程：生产者
        wait_for_shm(SHM_CHUNK_NAME);

        int fd = shm_open(SHM_CHUNK_NAME, O_RDWR, 0666);
        if (fd == -1) { perror("producer shm_open"); _exit(1); }
        void* addr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        if (addr == MAP_FAILED) { perror("producer mmap"); _exit(1); }

        ShmLayout layout = parse_shm(addr);
        size_t offset = 0;
        uint32_t seq = 0;

        while (offset < TEST_STREAM_SIZE) {
            uint32_t chunk_id = alloc_chunk(layout);
            Chunk& chunk = layout.chunk_pool[chunk_id];

            size_t remaining = TEST_STREAM_SIZE - offset;
            size_t this_len = std::min(CHUNK_SIZE, remaining);

            fill_test_payload(chunk, seq, offset, this_len);

            ChunkDesc desc{};
            desc.chunk_id = chunk_id;
            desc.payload_len = static_cast<uint32_t>(this_len);
            desc.crc32 = crc32(chunk.data, this_len);
            desc.flags = (offset + this_len >= TEST_STREAM_SIZE) ? 1u : 0u;
            push_desc(layout, desc);

            offset += this_len;
            ++seq;
        }

        munmap(addr, shm_size);
        _exit(0);
    }

    // 父进程：消费者 + 初始化
    int fd = shm_open(SHM_CHUNK_NAME, O_CREAT | O_RDWR, 0666);
    REQUIRE(fd != -1);
    REQUIRE(ftruncate(fd, shm_size) != -1);
    void* addr = mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    REQUIRE(addr != MAP_FAILED);

    ShmLayout layout = parse_shm(addr);
    init_shm(layout);

    size_t total_bytes = 0;
    uint32_t total_chunks = 0;
    uint32_t errors = 0;
    uint32_t expected_seq = 0;
    bool finished = false;

    while (!finished) {
        ChunkDesc desc{};
        if (!pop_desc(layout, desc)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // 零拷贝读取
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

        return_chunk(layout, desc.chunk_id);
        total_bytes += desc.payload_len;
        ++total_chunks;
        ++expected_seq;

        if (desc.flags & 1u) {
            finished = true;
        }
    }

    CHECK(total_bytes == TEST_STREAM_SIZE);
    CHECK(total_chunks == ((TEST_STREAM_SIZE + CHUNK_SIZE - 1) / CHUNK_SIZE));
    CHECK(errors == 0);

    int status = 0;
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);

    munmap(addr, shm_size);
    shm_unlink(SHM_CHUNK_NAME);
}
