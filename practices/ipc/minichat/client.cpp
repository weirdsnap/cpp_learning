// MiniChat client：连接 UDS server，发送一条消息并接收 ACK

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "common.hpp"

int main(int argc, char* argv[]) {
    const char* text = (argc > 1) ? argv[1] : "Hello from MiniChat client";

    // 1. 创建 UDS socket
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. 连接到 server 的 socket 文件
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, MINICHAT_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("connect");
        close(fd);
        return 1;
    }

    std::cout << "[MiniChat Client] 已连接到 " << MINICHAT_SOCK_PATH << "\n";

    // 3. 发送消息。SendMsg 会自动处理长度头和短写
    if (!SendMsg(fd, text)) {
        std::cerr << "[MiniChat Client] 发送失败\n";
        close(fd);
        return 1;
    }

    // 4. 接收 ACK
    std::string reply;
    if (RecvMsg(fd, reply)) {
        std::cout << "[MiniChat Client] 收到: " << reply << "\n";
    } else {
        std::cerr << "[MiniChat Client] 接收失败\n";
    }

    close(fd);
    return 0;
}
