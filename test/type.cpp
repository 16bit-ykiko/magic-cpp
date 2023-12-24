#include <iostream>
#include <magic/type.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace magic;

struct Point
{
};

static_assert(name_of<std::string>() == "std::string");
static_assert(name_of<const std::string&>() == "const std::string&");
static_assert(name_of<std::size_t>() == "std::size_t");
static_assert(name_of<std::vector<int>>() == "std::vector<int>");
static_assert(name_of<Point>() == "Point");

int main()
{
    std::cout << tree_of<std::make_index_sequence<10>>() << std::endl;
    std::cout << tree_of<std::map<int, std::string>>() << std::endl;
}