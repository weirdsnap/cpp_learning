// TinyRPC 集成测试
// fork 出 server 子进程，父进程作为 client 调用 Add(3, 5)，验证返回 8。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../third_party/doctest/doctest.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <string>

#include "rpc_protocol.hpp"

static void run_server() {
    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd == -1) _exit(1);

    unlink(TINYRPC_SOCK_PATH);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, TINYRPC_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1 ||
        listen(listen_fd, 1) == -1) {
        _exit(1);
    }

    int conn_fd = accept(listen_fd, nullptr, nullptr);
    if (conn_fd == -1) _exit(1);

    std::string request;
    if (RecvRpc(conn_fd, request)) {
        // 简单解析：提取 a 和 b
        int a = 0, b = 0;
        auto pa = request.find("\"a\":");
        auto pb = request.find("\"b\":");
        if (pa != std::string::npos && pb != std::string::npos) {
            a = std::atoi(request.c_str() + pa + 4);
            b = std::atoi(request.c_str() + pb + 4);
        }
        std::string response = "{\"result\":" + std::to_string(a + b) + "}";
        SendRpc(conn_fd, response);
    }

    close(conn_fd);
    close(listen_fd);
    unlink(TINYRPC_SOCK_PATH);
    _exit(0);
}

TEST_CASE("tinyrpc Add method over UDS") {
    pid_t pid = fork();
    REQUIRE(pid != -1);

    if (pid == 0) {
        run_server();
    }

    usleep(200 * 1000);

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    REQUIRE(fd != -1);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, TINYRPC_SOCK_PATH, sizeof(addr.sun_path) - 1);

    bool connected = (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);
    CHECK(connected);

    std::string request = "{\"method\":\"Add\",\"a\":3,\"b\":5}";
    CHECK(SendRpc(fd, request));

    std::string response;
    CHECK(RecvRpc(fd, response));
    CHECK(response == "{\"result\":8}");

    close(fd);

    int status = 0;
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);
}
