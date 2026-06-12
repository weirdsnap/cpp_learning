#include "shm_data.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    int fd = shm_open(NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
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

    std::cout << "[Reader] 开始消费..." << std::endl;

    int consumed = 0;
    while (consumed < 1000) {
        int write_idx = data->write_idx.load();
        int read_idx = data->read_idx.load();

        // 如果没有新数据，等待
        if (read_idx >= write_idx) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        int idx = read_idx % MSG_COUNT;
        const Message& msg = data->msgs[idx];

        std::cout << "[Reader] [" << consumed << "] id=" << msg.id
                  << ", name=" << msg.name
                  << ", value=" << msg.value << std::endl;

        data->read_idx.fetch_add(1);
        consumed++;

        // 模拟消费速度：每 30ms 读一条（比写端快）
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    std::cout << "[Reader] 消费完成，共 " << consumed << " 条" << std::endl;

    munmap(addr, SHM_SIZE);

    return 0;
}
