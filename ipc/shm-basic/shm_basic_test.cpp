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

TEST_CASE("shm-basic 写入并读取消息") {
    constexpr const char* shm_name = "my_shm";
    constexpr const char* test_msg = "TestMessage123";

    // 清理可能残留的共享内存
    std::system("shm_unlink my_shm 2>/dev/null");

    // 后台启动 writer，写入测试消息，2 秒后自动清理
    std::string writer_cmd = "./shm_basic_writer --test '";
    writer_cmd += test_msg;
    writer_cmd += "' > writer.log 2>&1 &";
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
    std::string output = exec("./shm_basic_reader");

    // 验证读取到消息
    CHECK(output.find(test_msg) != std::string::npos);

    // 等待 writer 清理并退出
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    // 再次清理以防万一
    std::system("shm_unlink my_shm 2>/dev/null");
}
