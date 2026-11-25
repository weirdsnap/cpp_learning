// MiniChat 集成测试
// fork 出 server 子进程，父进程作为 client 验证协议帧收发。

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../third_party/doctest/doctest.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <string>

#include "common.hpp"

static void run_server() {
    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd == -1) _exit(1);

    unlink(MINICHAT_SOCK_PATH);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, MINICHAT_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1 ||
        listen(listen_fd, 1) == -1) {
        _exit(1);
    }

    int conn_fd = accept(listen_fd, nullptr, nullptr);
    if (conn_fd == -1) _exit(1);

    std::string msg;
    if (RecvMsg(conn_fd, msg)) {
        SendMsg(conn_fd, "ACK: " + msg);
    }

    close(conn_fd);
    close(listen_fd);
    unlink(MINICHAT_SOCK_PATH);
    _exit(0);
}

TEST_CASE("minichat UDS echo with framed protocol") {
    pid_t pid = fork();
    REQUIRE(pid != -1);

    if (pid == 0) {
        run_server();
    }

    // 等待 server 启动
    usleep(200 * 1000);

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    REQUIRE(fd != -1);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, MINICHAT_SOCK_PATH, sizeof(addr.sun_path) - 1);

    bool connected = (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);
    CHECK(connected);

    std::string request = "hello minichat";
    bool sent = SendMsg(fd, request);
    CHECK(sent);

    std::string reply;
    bool received = RecvMsg(fd, reply);
    CHECK(received);
    CHECK(reply == "ACK: " + request);

    close(fd);

    int status = 0;
    waitpid(pid, &status, 0);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);
}
