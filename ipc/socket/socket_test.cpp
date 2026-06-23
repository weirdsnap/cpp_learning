// socket echo server/client 集成测试

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../third_party/doctest/doctest.h"

constexpr int TEST_PORT = 34567;
constexpr const char* TEST_MSG = "hello from doctest\n";

static void run_server() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("server socket");
        _exit(1);
    }

    int reuse = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(TEST_PORT);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1 ||
        listen(listen_fd, 1) == -1) {
        perror("server bind/listen");
        _exit(1);
    }

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (conn_fd == -1) {
        perror("server accept");
        _exit(1);
    }

    char buffer[1024];
    ssize_t n = read(conn_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        write(conn_fd, buffer, static_cast<size_t>(n));
    }

    close(conn_fd);
    close(listen_fd);
    _exit(0);
}

static std::string run_client() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("client socket");
        return {};
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("client connect");
        close(fd);
        return {};
    }

    write(fd, TEST_MSG, std::strlen(TEST_MSG));

    char buffer[1024];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (n > 0) {
        buffer[n] = '\0';
        return buffer;
    }
    return {};
}

TEST_CASE("socket echo server/client") {
    pid_t pid = fork();
    REQUIRE(pid != -1);

    if (pid == 0) {
        run_server();
    }

    // 给服务器一点时间启动并开始监听
    usleep(200 * 1000);

    std::string response = run_client();

    int status = 0;
    waitpid(pid, &status, 0);

    CHECK(response == TEST_MSG);
    CHECK(WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 0);
}
