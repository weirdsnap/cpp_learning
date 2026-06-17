#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <thread>

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

TEST_CASE("shm-basic 写入并读取消息") {
    // 清理可能残留的共享内存
    std::system("shm_unlink my_shm 2>/dev/null");

    // 后台启动 writer，写入测试消息，2 秒后自动清理
    std::string writer_cmd = "./shm_basic_writer --test 'TestMessage123' > writer.log 2>&1 &";
    std::system(writer_cmd.c_str());

    // 等待 writer 创建共享内存
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // 启动 reader
    std::string output = exec("./shm_basic_reader");

    // 验证读取到消息
    CHECK(output.find("TestMessage123") != std::string::npos);

    // 等待 writer 清理并退出
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    // 再次清理以防万一
    std::system("shm_unlink my_shm 2>/dev/null");
}
