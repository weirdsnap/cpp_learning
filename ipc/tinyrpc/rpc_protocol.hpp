// TinyRPC 阶段一：最小 RPC 协议定义
//
// 协议帧：固定 4 字节长度头（payload 长度，网络字节序）+ JSON payload
//
// 请求示例：{"method":"Add","a":1,"b":2}
// 响应示例：{"result":3}
//
// 阶段一先不用 protobuf，用 JSON 降低心智负担；后续把 payload 替换为 protobuf
// 即可复用同一套帧结构。

#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <string>
#include <unistd.h>

inline constexpr const char* TINYRPC_SOCK_PATH = "/tmp/tinyrpc.sock";

// 发送 len 字节，处理短写
inline bool SendAll(int fd, const void* buf, size_t len) {
    const char* p = static_cast<const char*>(buf);
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n <= 0) return false;
        p += n;
        len -= static_cast<size_t>(n);
    }
    return true;
}

// 接收 len 字节，处理短读
inline bool RecvAll(int fd, void* buf, size_t len) {
    char* p = static_cast<char*>(buf);
    while (len > 0) {
        ssize_t n = read(fd, p, len);
        if (n <= 0) return false;
        p += n;
        len -= static_cast<size_t>(n);
    }
    return true;
}

// 发送一条 RPC 消息：4 字节长度头 + JSON payload
inline bool SendRpc(int fd, const std::string& json) {
    uint32_t net_len = htonl(static_cast<uint32_t>(json.size()));
    if (!SendAll(fd, &net_len, sizeof(net_len))) return false;
    if (!json.empty() && !SendAll(fd, json.data(), json.size())) return false;
    return true;
}

// 接收一条 RPC 消息
inline bool RecvRpc(int fd, std::string& json) {
    uint32_t net_len = 0;
    if (!RecvAll(fd, &net_len, sizeof(net_len))) return false;

    uint32_t len = ntohl(net_len);
    json.resize(len);
    if (len > 0 && !RecvAll(fd, json.data(), len)) return false;
    return true;
}
