#include <cassert>
#include <iostream>
#include <magic/struct.h>
#include <vector>

using namespace magic;



struct Complex
{
    std::string name;
    std::vector<int> vec;
};
namespace magic
{
    template<>
    struct type_info<Complex>
    {
        constexpr static auto count = 2;
    };
}

// field_count_of 获取结构体的成员数量
static_assert(field_count_of<Complex>() == 2);

// field_types_of 获取结构体的成员类型集合 返回一个 tuple
static_assert(std::same_as<field_types_of<Complex>, std::tuple<std::string, std::vector<int>>>);

// field_type_of 获取指定位置的成员类型
static_assert(std::same_as<field_type_of<Complex, 0>, std::string>);
static_assert(std::same_as<field_type_of<Complex, 1>, std::vector<int>>);

// field_names_of 获取成员字段名 返回值类型是 std::array<std::string_view, N>
static_assert(field_names_of<Complex>() == std::array<std::string_view, 2>{"name", "vec"});

// field_name_of(size_t I) 获取指定位置的成员字段名
static_assert(field_name_of<Complex>(0) == "name");
static_assert(field_name_of<Complex>(1) == "vec");

int main()
{
    // field_of(auto&&) 获取指定位置的成员
    auto complex = Complex{
        "hello",
        {1, 2, 3},
    };
    std::cout << field_of<0>(complex) << std::endl;
    std::cout << field_of<1>(complex)[0] << std::endl;
    // output:
    // hello
    // 1

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
