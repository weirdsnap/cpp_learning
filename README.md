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
│   ├── nullptr_void/       # NULL / nullptr / void*
│   ├── string_ops/         # std::string 三种操作风格（C风格/成员函数/STL算法）
│   ├── bool_conversion/    # 隐式 bool 转换 / explicit operator bool / Safe Bool Idiom
│   ├── integer_promotion/  # 整数提升与通常算术转换（ch01/28）
│   ├── type_alias/         # 类型别名 using / typedef（ch01/29）
│   └── udl/                # 用户自定义字面量（UDL）
├── object_model/           # 对象模型与内存布局相关验证（博客 ch02）
│   ├── virtual_dispatch/   # vtable/vptr、构造/析构中虚调用、默认参数、final
│   ├── name_hiding/        # 名字隐藏、数组引用 vs 指针重载
│   ├── conversion_functions/ # 转换函数 explicit / const / 二义性
│   ├── nvi/                # NVI（Non-Virtual Interface）惯用法
│   ├── virtual_inheritance/ # 虚继承与菱形继承
│   ├── runtime_polymorphism/ # 运行时多态概述 + vtable/vptr 布局（博客 ch05）
│   ├── override_rtti/      # override / typeid / dynamic_cast（博客 ch05）
│   ├── pimpl/              # Pimpl 惯用法与编译防火墙
│   └── vptr_vbptr/         # vptr / vbptr 布局与虚继承内存模型
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
│   ├── dependent_base/     # 模板基类依赖名查找
│   └── if_constexpr/       # if constexpr 与 IFNDR 陷阱（ch04/25）
├── concurrency/            # 并发与内存模型相关验证（博客 ch06）
│   ├── basic_sync/         # mutex / lock_guard / unique_lock / atomic / 内存序
│   ├── atomic_ref/         # std::atomic_ref 与共享内存原子操作（C++20）
│   └── sleep/              # 并发中的睡眠与定时等待
├── exception_safety/       # 异常安全与未定义行为（博客 ch08）+ 编译期元编程（博客 ch09）
│   └── exception_ub/       # noexcept / 析构异常 / UB / Copy-and-Swap / placement new
├── io_stream/              # 输入输出与格式化（博客 ch07）
│   ├── output_basics/      # operator<<、格式化、精度、进制、boolalpha
│   ├── printf_format/      # printf / std::format 格式化（C++20）
│   └── stringstream/       # std::stringstream 与类型安全转换
├── bit_manipulation/       # 位运算惯用法
│   └── basic_ops/          # 2 的幂、lowbit、位掩码、对齐、popcount
├── leetcode/               # 算法题实现（博客 posts/leetcode/ch*）
│   ├── lc4_median_sorted_arrays/       # LC 4 寻找两个正序数组的中位数
│   ├── lc10_regular_expression_matching/ # LC 10 正则表达式匹配
│   ├── lc14_longest_common_prefix/     # LC 14 最长公共前缀
│   ├── lc42_trapping_rain_water/       # LC 42 接雨水
│   ├── lc135_candy/                    # LC 135 分发糖果
│   ├── lc2095_delete_middle_node/      # LC 2095 删除链表的中间节点
│   ├── lc221_maximal_square/           # LC 221 最大正方形
│   ├── lc3558_assign_edge_weights/     # LC 3558 边权赋值方案数 I
│   ├── lc3559_assign_edge_weights/     # LC 3559 边权赋值方案数 II
│   └── lc3612_process_string/          # LC 3612 用特殊操作处理字符串 I
└── ipc/                    # 进程间通信主题（博客 rpc/ch01、cpp/practices/ipc_*）
    ├── shm-basic/          # POSIX 共享内存基础示例
    ├── shm-ringbuffer/     # POSIX 共享内存环形缓冲区
    ├── shm-chunk/          # 基于 Chunk 池的共享内存生产者/消费者/压测
    ├── socket/             # TCP echo server/client
    ├── minichat/           # Unix Domain Socket 帧协议回显
    └── tinyrpc/            # UDS + JSON RPC 骨架
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

1. 在合适的主题目录下创建子文件夹，比如 `ipc/your-demo/`、`basic_type/your-topic/`、`object_model/your-topic/`、`memory_mgmt/your-topic/`、`data_validation/your-topic/`、`templates/your-topic/`、`bit_manipulation/your-topic/`、`leetcode/your-problem/` 等
2. 子文件夹内创建 `Makefile`
3. 根 `Makefile` 会自动发现 `ipc/*`、`basic_type/*`、`object_model/*`、`memory_mgmt/*`、`data_validation/*`、`templates/*`、`bit_manipulation/*`、`concurrency/*`、`exception_safety/*`、`io_stream/*` 和 `leetcode/*` 下的所有子目录
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
