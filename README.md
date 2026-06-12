# 博客 C++ 代码实现仓库

本仓库用于存放博客文章中各种 C++ 技术的实现代码，每个子项目都可以独立编译和运行。

## 目录结构

```
.
├── Makefile                # 根 Makefile，一键编译所有子项目
├── README.md               # 本文件
├── notes/                  # 博客笔记/文章
│   ├── POSIX共享内存学习笔记.md
│   └── POSIX共享内存进阶——环形缓冲区设计.md
└── ipc/                    # 进程间通信主题
    ├── shm-basic/          # POSIX 共享内存基础示例
    │   ├── Makefile
    │   ├── writer.cpp
    │   └── reader.cpp
    └── shm-ringbuffer/     # POSIX 共享内存环形缓冲区
        ├── Makefile
        ├── include/
        │   └── shm_data.h
        ├── writer.cpp
        └── reader.cpp
```

## 编译要求

- GCC/Clang 支持 C++17
- Linux 环境（使用了 POSIX 共享内存 API）
- `make`

## 编译所有项目

```bash
make
```

## 编译单个项目

```bash
cd ipc/shm-basic
make

cd ../shm-ringbuffer
make
```

## 运行示例

### POSIX 共享内存基础

```bash
cd ipc/shm-basic

# 终端 1
./shm_basic_writer

# 终端 2
./shm_basic_reader
```

### POSIX 共享内存环形缓冲区

```bash
cd ipc/shm-ringbuffer

# 终端 1
./shm_ringbuffer_writer

# 终端 2
./shm_ringbuffer_reader
```

## 添加新的子项目

1. 在合适的主题目录下创建子文件夹，比如 `ipc/your-demo/`
2. 子文件夹内创建 `Makefile`
3. 根 `Makefile` 会自动发现 `ipc/*` 下的所有子目录
4. 运行 `make` 即可编译新项目

## 清理编译产物

```bash
make clean
```
