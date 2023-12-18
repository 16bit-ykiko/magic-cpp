#include "magic/type.h"
#include <array>
#include <cassert>
#include <iostream>
#include <magic/struct.h>
#include <utility>
#include <vector>

using namespace magic;

struct Complex
{
    std::pair<int, int> pair;
    std::string name;
    int arr[3];
    std::array<std::size_t, 4> arr2;
    std::vector<int> vec;
};

// field_count_of 获取结构体的成员数量
static_assert(field_count_of<Complex>() == 5);

// field_types_of 获取结构体的成员类型集合 返回一个 tuple
static_assert(std::same_as<field_types_of<Complex>,
                           std::tuple<std::pair<int, int>, std::string, int[3], std::array<std::size_t, 4>, std::vector<int>>>);

// field_type_of 获取指定位置的成员类型
static_assert(std::same_as<field_type_of<Complex, 0>, std::pair<int, int>>);
static_assert(std::same_as<field_type_of<Complex, 1>, std::string>);
static_assert(std::same_as<field_type_of<Complex, 2>, int[3]>);
static_assert(std::same_as<field_type_of<Complex, 3>, std::array<std::size_t, 4>>);
static_assert(std::same_as<field_type_of<Complex, 4>, std::vector<int>>);

// field_names_of 获取成员字段名 返回值类型是 std::array<std::string_view, N>
static_assert(field_names_of<Complex>() == std::array<std::string_view, 5>{
                                               "pair",
                                               "name",
                                               "arr",
                                               "arr2",
                                               "vec",
                                           });

// field_name_of(size_t I) 获取指定位置的成员字段名
static_assert(field_name_of<Complex>(0) == "pair");
static_assert(field_name_of<Complex>(1) == "name");
static_assert(field_name_of<Complex>(2) == "arr");
static_assert(field_name_of<Complex>(3) == "arr2");
static_assert(field_name_of<Complex>(4) == "vec");

int main()
{
    // field_of(auto&&) 获取指定位置的成员
    Complex complex{
        {1, 2},
        "hello",
        {1, 2, 3},
        {1, 2, 3, 4},
        {1, 2, 3},
    };
    std::cout << field_of<0>(complex).first << std::endl;
    std::cout << field_of<1>(complex) << std::endl;
    // output:
    // 1
    // hello

    using types = field_types_of<Complex>;
    std::cout << full_tree_of<types>() << std::endl;

    auto f = [](auto field)
    {
        constexpr auto index = field.index();
        constexpr auto name = field.name();
        using type = decltype(field)::type;
        if constexpr (name == "name")
        {
            std::cout << index << ": " << name << " " << field.value() << std::endl;
        }
        else if constexpr (name == "vec")
        {
            std::cout << index << ": " << name << " " << field.value()[0] << field.value()[1] << field.value()[2] << std::endl;
        }
    };

    magic::foreach (complex, f);
    // output:
    // 0: hello hello
    // 1: vec 123
}
