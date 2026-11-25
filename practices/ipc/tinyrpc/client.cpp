// TinyRPC client：阶段一最小骨架
//
// 构造 JSON 请求，通过协议帧发送给 server，接收并打印 JSON 响应。

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "rpc_protocol.hpp"

int main(int argc, char* argv[]) {
    int a = (argc > 1) ? std::atoi(argv[1]) : 1;
    int b = (argc > 2) ? std::atoi(argv[2]) : 2;

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, TINYRPC_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("connect");
        close(fd);
        return 1;
    }

    // 构造请求 JSON
    std::string request = "{\"method\":\"Add\",\"a\":" + std::to_string(a) +
                          ",\"b\":" + std::to_string(b) + "}";

    if (!SendRpc(fd, request)) {
        std::cerr << "[TinyRPC Client] 发送失败\n";
        close(fd);
        return 1;
    }

    std::string response;
    if (RecvRpc(fd, response)) {
        std::cout << "[TinyRPC Client] 响应: " << response << "\n";
    } else {
        std::cerr << "[TinyRPC Client] 接收失败\n";
    }

    close(fd);
    return 0;
}
