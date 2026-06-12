#include "shm_data.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    void* addr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    Data* data = static_cast<Data*>(addr);

    // 初始化（如果是新创建的共享内存）
    data->write_idx.store(0);
    data->read_idx.store(0);

    std::cout << "[Writer] 开始生产..." << std::endl;

    for (int i = 0; i < 1000; ++i) {
        // 如果写端领先读端超过 MSG_COUNT，阻塞等待
        while (data->write_idx.load() - data->read_idx.load() >= MSG_COUNT) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        int idx = data->write_idx.load() % MSG_COUNT;
        data->msgs[idx].id = i;
        std::strcpy(data->msgs[idx].name, "temperature");
        data->msgs[idx].value = 36.5 + i * 0.0001;

        // 数据写完后，更新写指针（通知读端）
        data->write_idx.fetch_add(1);

        if (i % 10 == 0) {
            std::cout << "[Writer] 已生产 " << i << " 条" << std::endl;
        }

        // 模拟生产速度：每 50ms 写一条
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "[Writer] 完成 1000 条，按回车键退出..." << std::endl;
    std::cin.get();

    munmap(addr, SHM_SIZE);
    shm_unlink(NAME);

    return 0;
}
