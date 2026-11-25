#include <fcntl.h>       // shm_open()，以及 O_RDONLY 标志位
#include <sys/mman.h>    // mmap()、munmap()，以及 MAP_SHARED、MAP_FAILED、PROT_READ
#include <sys/stat.h>    // 文件权限相关定义，代码中直接用了 0666
#include <unistd.h>      // close()
#include <iostream>      // std::cout、std::endl
#include <string_view>   // std::string_view：零拷贝的字符串视图

int main() {
    constexpr const char* name = "my_shm";
    constexpr size_t SIZE = 4096;

    // 1. 打开共享内存（只读，不创建）
    int fd = shm_open(name, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. 映射到进程地址空间（只读）
    void* addr = mmap(nullptr, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // mmap 成功后 fd 即可关闭
    close(fd);

    // 3. 读取并打印（用 string_view 避免依赖 \0 终止符）
    std::string_view data(static_cast<const char*>(addr), SIZE);
    std::cout << "[Reader] 读取: " << data << std::endl;

    // 4. 解除映射（不调用 shm_unlink，由写入端负责清理）
    munmap(addr, SIZE);

    return 0;
}
