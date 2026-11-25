#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>

static std::string exec(const char* cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

static bool shm_exists(const char* name) {
    int fd = shm_open(name, O_RDONLY, 0666);
    if (fd != -1) {
        close(fd);
        return true;
    }
    return false;
}

TEST_CASE("shm-ringbuffer 生产消费 10 条消息") {
    constexpr const char* shm_name = "/my_shm_ringbuffer";
    constexpr int count = 10;

    // 清理可能残留的共享内存
    std::system("shm_unlink /my_shm_ringbuffer 2>/dev/null");

    // 后台启动 writer，生产 count 条消息后自动退出并清理
    std::string writer_cmd = "./shm_ringbuffer_writer.out " + std::to_string(count) + " > writer.log 2>&1 &";
    std::system(writer_cmd.c_str());

    // 等待 writer 创建共享内存（最多 5 秒）
    bool ready = false;
    for (int i = 0; i < 50; ++i) {
        if (shm_exists(shm_name)) {
            ready = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    CHECK(ready);

    // 启动 reader
    std::string output = exec(("./shm_ringbuffer_reader.out " + std::to_string(count)).c_str());

    // 验证消费完成
    CHECK(output.find("消费完成，共 10 条") != std::string::npos);

    // 等待 writer 清理并退出
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // 再次清理以防万一
    std::system("shm_unlink /my_shm_ringbuffer 2>/dev/null");
}
