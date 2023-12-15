# Magic C++
`magic-c++`是一个轻量级`C++20`的`header-only`的库，利用一些特殊的技巧和模板元编程，提供了一些强大的功能。
- 对类型的反射（获取类型名，可视化类型）
- 对聚合结构体的反射（遍历结构体，根据索引获取结构体成员，获取字段名）
- 对枚举的反射（获取枚举名）

# 目录
- [Magic C++](#magic-c)
- [目录](#目录)
- [Example](#example)
  - [可视化类型](#可视化类型)
    - [示例一](#示例一)
    - [示例二](#示例二)
    - [自定义类型名](#自定义类型名)
  - [结构体反射](#结构体反射)
    - [example](#example-1)
    - [限制\&建议](#限制建议)
  - [枚举反射 -- Todo](#枚举反射----todo)
    - [获取枚举名](#获取枚举名)
    - [遍历枚举](#遍历枚举)
    - [位域枚举](#位域枚举)
- [安装](#安装)
- [编译器支持](#编译器支持)
- [限制](#限制)
  

# Example
## 可视化类型
包含`magic/type.h`即可使用结构体下面这些功能
### 示例一
```cpp
#include <iostream>
#include <magic/type.h>

int main()
{
    using T = int (*(*(*)(int*))[4])(int*);
    std::cout << magic::tree_of<T>() << std::endl;
}
```
输出

![指针](doc/ptr1.png)

测试
```c++
using A = int(*)(int*);
using B = A[4];
using C = B*;
using D = C(*)(int*);
static_assert(std::is_same_v<D, T>);
```
### 示例二
```cpp
#include <iostream>
#include <magic/type.h>

int main()
{
    using T = std::map<int, std::string>;
    std::cout << magic::full_tree_of<T>() << std::endl;
}
```
![map](doc/map.png)

### 自定义类型名
注意到上面在打印`std::string`的时候把它全部的内容都显示出来了，不好阅读，没关系，你可以通过自定义的特化来实现自定义类型名
```cpp
namespace magic
{
    template<>
    struct type_info<std::string>
    {
        constexpr static auto value = "std::string";
    };
}

int main()
{
    std::cout << magic::tree_of<const std::string&>() << std::endl;
     std::cout << magic::tree_of<std::map<int, std::string>>() << std::endl;
}
```
输出
![map](doc/string.png)，方便起见，我已经预定义了一些特化方便使用，如果你不想受到特化的影响，只想类型全部的内容，可以使用`magic::full_tree_of`。

## 结构体反射
包含`magic/struct.h`即可使用结构体下面这些功能

### example
```c++
struct Point
{
    int x;
    int y;
};

using namespace magic;

// template <typename T>
// field_num_of 获取结构体的成员数量
static_assert(field_num_of<Point>() == 2);

// template <typename T>
// field_types_of 获取结构体的成员类型集合 返回一个 tuple
static_assert(std::is_same_v<field_types_of<Point>, std::tuple<int, int>>);

// template <typenameT, size_t I>
// field_type_of 获取指定位置的成员类型
static_assert(std::is_same_v<field_type_of<Point, 0>, int>);
static_assert(std::is_same_v<field_type_of<Point, 1>, int>);

// template <typename T>
// field_names_of 获取成员字段名 返回值类型是 std::array<std::string_view, N>
static_assert(field_names_of<Point>() == std::array<std::string_view, 2>{"x", "y"});

// template <typename T>
// field_name_of(size_t I) 获取指定位置的成员字段名 
static_assert(field_name_of<Point>(0) == "x");
static_assert(field_name_of<Point>(1) == "y");

// template <typename T>
// field_of(auto&&) 获取指定位置的成员
constexpr auto point = Point{1, 2};
static_assert(field_of<0>(point) == 1);
static_assert(field_of<1>(point) == 2);

int main()
{
    Point point = {1, 2};
    auto f = [](auto name, auto&& field)
    {
        std::cout << name << " = " << field << std::endl;
    };
    // foreach 接受一个回调函数遍历结构体
    magic::foreach (point, f);
    // output:
    // x = 1
    // y = 2
}
```
### 限制&建议

## 枚举反射 -- Todo 
### 获取枚举名

### 遍历枚举

### 位域枚举

# 安装
只需要把`include`包含进你的项目即可
# 编译器支持

# 限制