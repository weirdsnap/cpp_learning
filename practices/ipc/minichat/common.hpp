// MiniChat 公共协议定义
//
// 协议格式：固定 4 字节头（payload 长度，网络字节序）+ 变长 payload
// 这种设计解决了 TCP 流式传输中的"粘包/拆包"问题：
// 接收方先读 4 字节得到消息长度，再读对应长度的 payload，就能准确知道一条消息的边界。

#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <unistd.h>

// socket 路径，同机进程通过文件系统路径找到对方
inline constexpr const char* MINICHAT_SOCK_PATH = "/tmp/minichat.sock";

// 发送 len 字节，直到全部发送完毕或出错才返回。
// TCP 是流式协议，send/write 一次不一定能把所有字节发出去，
// 必须循环发送并偏移指针，这就是"短写"处理。
inline bool SendAll(int fd, const void* buf, size_t len) {
    const char* p = static_cast<const char*>(buf);
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n <= 0) return false;  // 对端关闭或出错
        p += n;
        len -= static_cast<size_t>(n);
    }
    return true;
}

// 接收 len 字节，直到全部收完或出错才返回。
// 对应"短读"处理：内核可能一次只给一部分数据，必须循环读。
inline bool RecvAll(int fd, void* buf, size_t len) {
    char* p = static_cast<char*>(buf);
    while (len > 0) {
        ssize_t n = read(fd, p, len);
        if (n <= 0) return false;  // 对端关闭或出错
        p += n;
        len -= static_cast<size_t>(n);
    }
    return true;
}

// 发送一条完整消息：4 字节网络字节序长度 + payload
inline bool SendMsg(int fd, const std::string& msg) {
    uint32_t net_len = htonl(static_cast<uint32_t>(msg.size()));
    if (!SendAll(fd, &net_len, sizeof(net_len))) return false;
    if (!msg.empty() && !SendAll(fd, msg.data(), msg.size())) return false;
    return true;
}

// 接收一条完整消息
inline bool RecvMsg(int fd, std::string& msg) {
    uint32_t net_len = 0;
    if (!RecvAll(fd, &net_len, sizeof(net_len))) return false;

    uint32_t len = ntohl(net_len);
    msg.resize(len);
    if (len > 0 && !RecvAll(fd, msg.data(), len)) return false;
    return true;
}
