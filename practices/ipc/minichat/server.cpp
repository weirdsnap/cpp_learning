// MiniChat server：基于 Unix Domain Socket 的字符串回显服务
//
// UDS（Unix Domain Socket）和 TCP Socket 的 API 几乎一样，区别只在：
// 1. 地址族用 AF_UNIX 而不是 AF_INET
// 2. 地址是文件系统路径（如 /tmp/minichat.sock）而不是 IP:port
// 3. 同机通信时 UDS 更快，因为不走网络协议栈

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "common.hpp"

int main() {
    // 1. 创建 UDS socket
    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. 绑定到文件路径
    // 如果上次运行没有清理 socket 文件，bind 会失败，所以先 unlink
    unlink(MINICHAT_SOCK_PATH);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, MINICHAT_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("bind");
        close(listen_fd);
        return 1;
    }

    // 3. 监听
    if (listen(listen_fd, 1) == -1) {
        perror("listen");
        close(listen_fd);
        return 1;
    }

    std::cout << "[MiniChat Server] 监听 " << MINICHAT_SOCK_PATH << "\n";

    // 4. 接受一个连接
    int conn_fd = accept(listen_fd, nullptr, nullptr);
    if (conn_fd == -1) {
        perror("accept");
        close(listen_fd);
        unlink(MINICHAT_SOCK_PATH);
        return 1;
    }

    std::cout << "[MiniChat Server] 客户端已连接\n";

    // 5. 使用协议帧收发消息
    std::string msg;
    while (RecvMsg(conn_fd, msg)) {
        std::cout << "[MiniChat Server] 收到: " << msg << "\n";

        if (msg == "quit") {
            std::cout << "[MiniChat Server] 收到 quit，退出\n";
            break;
        }

        // 回显 ACK
        if (!SendMsg(conn_fd, "ACK: " + msg)) {
            break;
        }
    }

    close(conn_fd);
    close(listen_fd);
    unlink(MINICHAT_SOCK_PATH);
    return 0;
}
