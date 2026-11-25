# 博客 C++ 代码实现仓库

[![CI](https://github.com/weirdsnap/cpp_learning/actions/workflows/ci.yml/badge.svg)](https://github.com/weirdsnap/cpp_learning/actions/workflows/ci.yml)

存放博客文章的验证与实现代码。每个子项目独立编译、独立测试。

## 章节映射（顶层目录 = 博客章节，一一对应）

| 仓库目录 | 博客章节 |
|---|---|
| `basic_type/` | cpp/ch01 语言基础与类型系统 |
| `object_model/` | cpp/ch02 对象模型与多态机制 |
| `memory_mgmt/` | cpp/ch03 内存管理与资源安全 |
| `templates/` | cpp/ch04 模板与泛型编程 |
| `concurrency/` | cpp/ch06 并发与内存模型 |
| `stdlib/` | cpp/ch07 标准库与工程实践（含 IO：output_basics / printf_format / stringstream） |
| `exception_safety/` | cpp/ch08 异常安全与未定义行为 |
| `metaprogramming/` | cpp/ch09 编译期元编程与深层机制（待补） |
| `misc/` | cpp/ch10 杂项与陷阱（位运算、UDL、sleep 等） |
| `practices/` | cpp/practices 专项实践（ipc、对象池、lambda 递归、type_identity） |
| `leetcode/` | leetcode 各子分类 |
| `data_validation/` | 预留给后端网络系列的差错控制篇（crc32 / hamming / parity_xor），文章未写 |

新增章节时两边同步：博客加章节，这里加同名映射目录并更新本表。

## 约定

- 子项目放在对应章节目录下，snake_case 命名；leetcode 用 `lc{题号}_{slug}`。
- 每个子项目的源文件首行注释标注对应的博客文章路径（如 `// 博客 ch06/06`）。
- 子项目结构：`<name>.cpp`（演示）+ `<name>_test.cpp`（doctest 测试）+ `Makefile`（含 `test` 目标）。
- 根 `Makefile` 用 `find` 自动发现各章节目录下任意深度的子项目，新增子项目不需要改根 Makefile。
- 测试只断言标准保证或可移植的行为，实现相关的具体数值（如 SBO 阈值）不断言。

## 编译要求

- GCC/Clang 支持 C++17（部分 leetcode 子项目用 C++23，CI 使用 g++-14）
- Linux 环境（practices/ipc 使用了 POSIX 共享内存 API）
- `make`

## 常用命令

```bash
make          # 编译所有子项目
make test     # 运行所有带 test 目标的子项目测试
make clean    # 清理编译产物

cd memory_mgmt/smart_pointers && make test   # 单个子项目
```

## practices/ipc 运行示例

```bash
cd practices/ipc/shm-basic
./shm_basic_writer.out   # 终端 1
./shm_basic_reader.out   # 终端 2
```

## 目录结构

```
.
├── Makefile                # 根 Makefile，find 自动发现子项目
├── third_party/doctest/    # doctest 单头文件测试框架
├── basic_type/  object_model/  memory_mgmt/  templates/  concurrency/
├── stdlib/  exception_safety/  metaprogramming/  misc/  practices/
├── data_validation/        # 预留：后端网络系列差错控制篇
└── leetcode/               # lc{题号}_{slug} 命名
```
