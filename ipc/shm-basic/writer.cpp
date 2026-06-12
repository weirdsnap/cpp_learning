#include <cstddef>       // NULL（C 风格空指针，C++11 后推荐用 nullptr）
#include <cstdio>        // perror()：根据 errno 打印错误信息
#include <fcntl.h>       // shm_open()，以及 O_CREAT、O_RDWR 等标志位
#include <sys/mman.h>    // mmap()、munmap()、shm_unlink()，以及 MAP_SHARED、MAP_FAILED、PROT_READ、PROT_WRITE
#include <sys/stat.h>    // 文件权限相关定义（如 S_IRUSR），代码中直接用了 0666
#include <unistd.h>      // close()、ftruncate()
#include <cstring>       // std::memcpy()、std::strlen()
#include <iostream>      // std::cout、std::cin、std::endl

int main() {
    constexpr const char* name = "my_shm";

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, 4096) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    void* addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    const char* msg = "Hello, SHM!";
    std::memcpy(addr, msg, std::strlen(msg) + 1);

    std::cout << "[Writer] 已写入，按回车键退出..." << std::endl;
    std::cin.get();

    munmap(addr, 4096);
    shm_unlink(name);

    return 0;
}
