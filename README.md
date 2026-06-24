# 博客 C++ 代码实现仓库

[![CI](https://github.com/weirdsnap/cpp_learning/actions/workflows/ci.yml/badge.svg)](https://github.com/weirdsnap/cpp_learning/actions/workflows/ci.yml)

本仓库用于存放博客文章中各种 C++ 技术的实现代码，每个子项目都可以独立编译和运行。

## 目录结构

```
.
├── Makefile                # 根 Makefile，一键编译所有子项目
├── README.md               # 本文件
├── notes/                  # 博客笔记/文章（软连接，不提交到本仓库）
├── third_party/            # 第三方依赖
│   └── doctest/            # doctest 单头文件测试框架
├── basic_type/             # 基础类型与类型系统相关验证（博客 ch01）
│   ├── fundamental_types/  # 无符号回绕、char 符号性、字面量、字符串字面量
│   ├── type_deduction/     # auto / decltype / decltype(auto) / 类型退化
│   ├── const_reference/    # const 引用、std::max/min 返回引用
│   ├── lifecycle/          # 构造析构顺序、const/constexpr、悬垂引用
│   ├── polymorphism/       # 静态/动态类型、enum class、对象切片
│   ├── function_lambda/    # operator bool、函数对象、Lambda、求值顺序
│   ├── modern_types/       # variant / optional / 转发引用
│   └── nullptr_void/       # NULL / nullptr / void*
├── object_model/           # 对象模型与内存布局相关验证（博客 ch02）
│   ├── virtual_dispatch/   # vtable/vptr、构造/析构中虚调用、默认参数、final
│   ├── name_hiding/        # 名字隐藏、数组引用 vs 指针重载
│   ├── conversion_functions/ # 转换函数 explicit / const / 二义性
│   ├── nvi/                # NVI（Non-Virtual Interface）惯用法
│   ├── virtual_inheritance/ # 虚继承与菱形继承
│   └── override_rtti/      # override / typeid / dynamic_cast（博客 ch05）
├── memory_mgmt/            # 内存管理与资源安全相关验证（博客 ch03）
│   ├── smart_pointers/     # shared_ptr / weak_ptr / unique_ptr / 自定义删除器
│   ├── ownership/          # 拷贝/移动、三/五/零法则、特殊成员函数
│   ├── move_elision/       # 移动语义、RVO/NRVO、返回值优化
│   ├── raii/               # new/delete vs malloc/free、RAII 代理与异常安全
│   └── initialization/     # 成员初始化顺序、模板静态成员按需实例化
├── data_validation/        # 数据校验与纠错
│   ├── crc32/              # CRC32：检错但不纠错
│   ├── parity_xor/         # 奇偶校验与 XOR 校验和
│   └── hamming/            # Hamming(7,4)：可纠正单比特错误
├── templates/              # 模板与泛型编程相关验证（博客 ch04）
│   ├── deduction/          # 模板参数推导、顶层 const、auto decay
│   ├── overload/           # 重载决议、模板精确匹配、SFINAE
│   ├── specialization/     # 全特化、偏特化
│   ├── forwarding/         # 万能引用、完美转发
│   ├── variadic/           # 可变参数模板、折叠表达式
│   └── dependent_base/     # 模板基类依赖名查找
├── concurrency/            # 并发与内存模型相关验证（博客 ch06）
│   └── basic_sync/         # mutex / lock_guard / unique_lock / atomic / 内存序
├── exception_safety/       # 异常安全与未定义行为（博客 ch08）+ 编译期元编程（博客 ch09）
│   └── exception_ub/       # noexcept / 析构异常 / UB / Copy-and-Swap / placement new
├── bit_manipulation/       # 位运算惯用法
│   └── basic_ops/          # 2 的幂、lowbit、位掩码、对齐、popcount
└── ipc/                    # 进程间通信主题
    ├── shm-basic/          # POSIX 共享内存基础示例
    └── shm-ringbuffer/     # POSIX 共享内存环形缓冲区
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

1. 在合适的主题目录下创建子文件夹，比如 `ipc/your-demo/`、`basic_type/your-topic/`、`object_model/your-topic/`、`memory_mgmt/your-topic/`、`data_validation/your-topic/`、`templates/your-topic/` 或 `bit_manipulation/your-topic/`
2. 子文件夹内创建 `Makefile`
3. 根 `Makefile` 会自动发现 `ipc/*`、`basic_type/*`、`object_model/*`、`memory_mgmt/*`、`data_validation/*`、`templates/*` 和 `bit_manipulation/*` 下的所有子目录
4. 运行 `make` 即可编译新项目

## 运行测试

部分子项目包含基于 [doctest](https://github.com/doctest/doctest) 的单元测试：

```bash
make test
```

测试文件命名为 `*_test.cpp`，对应的 Makefile 提供 `test` 目标。进入子目录也可以单独运行：

```bash
cd basic_type/fundamental_types
make test
```

## 清理编译产物

```bash
make clean
```
