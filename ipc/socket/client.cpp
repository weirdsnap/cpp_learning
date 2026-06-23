// 简单的 TCP echo client
// 连接 server，发送一条消息，接收并打印返回内容。

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
    const char* msg = (argc > 2) ? argv[2] : "hello socket\n";

    // 1. 创建 socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. 设置服务器地址
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
        perror("inet_pton");
        close(fd);
        return 1;
    }

    // 3. 连接服务器
    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("connect");
        close(fd);
        return 1;
    }

    std::cout << "[Client] 已连接到 127.0.0.1:" << port << "\n";

    // 4. 发送消息
    size_t msg_len = std::strlen(msg);
    ssize_t sent = write(fd, msg, msg_len);
    if (sent == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    // 5. 接收响应
    char buffer[1024];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "[Client] 收到: " << buffer;
    } else if (n == 0) {
        std::cout << "[Client] 服务器关闭连接\n";
    } else {
        perror("read");
    }

    close(fd);
    return 0;
}
