#include <iostream>
#include <magic/type.h>
#include <map>
#include <vector>

namespace magic
{
    template <>
    struct type_info<std::string>
    {
        constexpr static auto name = "std::string";
    };
} // namespace magic

struct A
{
    int x;
    void f(int) {}
};

int main()
{
    auto m = &A::f;
    using T = int (*(*(*)(int*))[4])(int*);
    std::cout << magic::tree_of<T>() << std::endl;
    std::cout << magic::tree_of<const std::string&>() << std::endl;
    std::cout << magic::tree_of<std::map<int, std::string>>() << std::endl;
    system("pause");
}