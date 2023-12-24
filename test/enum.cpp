#include "magic/enum.h"
#include <iostream>

using namespace magic;

// test for enum_start
enum class A
{
    START = 0,
};
static_assert(enum_start<A>() == 0);

enum class B
{
    START = -32,
};
static_assert(enum_start<B>() == -32);

enum class C
{
    START = 33,
};
static_assert(enum_start<C>() == 33);

// test for enum_max
enum class D
{
    START = 0,
    SECOND = 5,
    MAX = 10,
};

static_assert(enum_max<D>() == 10);

auto& operator<<(std::ostream& os, magic::string_view str)
{
    os << std::string_view(str.data(), str.size());
    return os;
}

enum class Color
{
    RED,
    GREEN,
    BLUE,
};

int main()
{
    magic::foreach<Color>([](auto field) { std::cout << "value: " << field.value() << " name: " << field.name() << std::endl; });
    return 0;
}