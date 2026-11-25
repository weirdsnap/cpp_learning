// TinyRPC server：阶段一最小骨架
//
// 这个 server 只做一件事：注册一个 Add 方法，接收 JSON 请求，返回 JSON 结果。
// 它展示了 RPC 的核心流程：
// 1. 接收协议帧
// 2. 反序列化请求（这里手动解析 JSON）
// 3. 根据 method 名字分发到对应函数
// 4. 序列化响应并发送

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "rpc_protocol.hpp"

// 阶段一：手动从 JSON 里提取 "a" 和 "b" 的值。
// 真实项目会用 nlohmann/json 或 protobuf，但这里为了聚焦 RPC 骨架而简化。
static bool ParseAddArgs(const std::string& json, int& a, int& b) {
    auto pos_a = json.find("\"a\":");
    auto pos_b = json.find("\"b\":");
    if (pos_a == std::string::npos || pos_b == std::string::npos) return false;

    a = std::atoi(json.c_str() + pos_a + 4);
    b = std::atoi(json.c_str() + pos_b + 4);
    return true;
}

static std::string HandleRequest(const std::string& json) {
    if (json.find("\"method\":\"Add\"") != std::string::npos ||
        json.find("\"method\": \"Add\"") != std::string::npos) {
        int a = 0, b = 0;
        if (ParseAddArgs(json, a, b)) {
            return "{\"result\":" + std::to_string(a + b) + "}";
        }
        return "{\"error\":\"invalid Add args\"}";
    }
    return "{\"error\":\"unknown method\"}";
}

int main() {
    int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return 1;
    }

    unlink(TINYRPC_SOCK_PATH);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, TINYRPC_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1 ||
        listen(listen_fd, 1) == -1) {
        perror("bind/listen");
        close(listen_fd);
        return 1;
    }

    std::cout << "[TinyRPC Server] 监听 " << TINYRPC_SOCK_PATH << "\n";

    int conn_fd = accept(listen_fd, nullptr, nullptr);
    if (conn_fd == -1) {
        perror("accept");
        close(listen_fd);
        unlink(TINYRPC_SOCK_PATH);
        return 1;
    }

    std::string request;
    if (RecvRpc(conn_fd, request)) {
        std::cout << "[TinyRPC Server] 收到请求: " << request << "\n";
        std::string response = HandleRequest(request);
        std::cout << "[TinyRPC Server] 返回响应: " << response << "\n";
        SendRpc(conn_fd, response);
    }

    close(conn_fd);
    close(listen_fd);
    unlink(TINYRPC_SOCK_PATH);
    return 0;
}
