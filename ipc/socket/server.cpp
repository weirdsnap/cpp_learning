// 简单的 TCP echo server
// 监听指定端口，接收一个连接，把收到的内容原样返回。
// 收到 "quit" 或客户端关闭连接时退出。

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int port = (argc > 1) ? std::atoi(argv[1]) : 12345;

    // 1. 创建监听 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. 允许端口复用，方便测试重启
    int reuse = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("setsockopt");
        close(listen_fd);
        return 1;
    }

    // 3. 绑定地址和端口
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1
    addr.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("bind");
        close(listen_fd);
        return 1;
    }

    // 4. 开始监听
    if (listen(listen_fd, 1) == -1) {
        perror("listen");
        close(listen_fd);
        return 1;
    }

    std::cout << "[Server] 监听 127.0.0.1:" << port << "\n";

    // 5. 接受一个连接
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (conn_fd == -1) {
        perror("accept");
        close(listen_fd);
        return 1;
    }

    std::cout << "[Server] 客户端已连接\n";

    // 6. echo 循环
    char buffer[1024];
    while (true) {
        ssize_t n = read(conn_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            std::cout << "[Server] 客户端断开连接\n";
            break;
        }
        buffer[n] = '\0';
        std::cout << "[Server] 收到: " << buffer;

        if (std::strncmp(buffer, "quit", 4) == 0) {
            std::cout << "[Server] 收到 quit，退出\n";
            break;
        }

        // 原样返回
        write(conn_fd, buffer, static_cast<size_t>(n));
    }

    close(conn_fd);
    close(listen_fd);
    return 0;
}
