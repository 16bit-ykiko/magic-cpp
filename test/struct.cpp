#include <array>
#include <cassert>
#include <iostream>
#include <magic/struct.h>
#include <type_traits>
#include <vector>

using namespace magic;
struct Point
{
    int x;
    int y;
};
/**
 * test for other functions
 */

static_assert(field_count_of<Point>() == 2);

static_assert(std::is_same_v<field_types_of<Point>, std::tuple<int, int>>);
static_assert(std::is_same_v<field_type_of<Point, 0>, int>);
static_assert(std::is_same_v<field_type_of<Point, 1>, int>);

static_assert(field_names_of<Point>() == std::array<std::string_view, 2>{"x", "y"});
static_assert(field_name_of<Point>(0) == "x");
static_assert(field_name_of<Point>(1) == "y");

constexpr auto point = Point{1, 2};
static_assert(field_of<0>(point) == 1);
static_assert(field_of<1>(point) == 2);

struct Container
{
    Point point;
    std::array<int, 3> array;
    std::vector<int> vec;
};

static_assert(field_count_of<Container>() == 3);

static_assert(std::is_same_v<field_types_of<Container>, std::tuple<Point, std::array<int, 3>, std::vector<int>>>);
static_assert(std::is_same_v<field_type_of<Container, 0>, Point>);
static_assert(std::is_same_v<field_type_of<Container, 1>, std::array<int, 3>>);
static_assert(std::is_same_v<field_type_of<Container, 2>, std::vector<int>>);

static_assert(field_names_of<Container>() == std::array<std::string_view, 3>{"point", "array", "vec"});
static_assert(field_name_of<Container>(0) == "point");
static_assert(field_name_of<Container>(1) == "array");
static_assert(field_name_of<Container>(2) == "vec");

void test_for_container()
{
    auto container = Container{
        {1, 2},
        {3, 4, 5},
        {6, 7, 8},
    };
    auto f = [](auto field)
    {
        constexpr auto name = field.name();
        if constexpr (name == "point")
        {
            assert(field.value().x == 1);
            assert(field.value().y == 2);
        }
        else if constexpr (name == "array")
        {
            assert(field.value()[0] == 3);
            assert(field.value()[1] == 4);
            assert(field.value()[2] == 5);
        }
        else if constexpr (name == "vec")
        {
            assert(field.value()[0] == 6);
            assert(field.value()[1] == 7);
            assert(field.value()[2] == 8);
        }
    };
    foreach (container, f)
        ;
}

// gcc 13 ice, so cannot get the fields number of struct which has reference
// type member in gcc 13
struct Reference
{
    int x;
    const int& ref;
    int y;
};

static_assert(field_count_of<Reference>() == 3);
static_assert(std::is_same_v<field_types_of<Reference>, std::tuple<int, const int&, int>>);
static_assert(std::is_same_v<field_type_of<Reference, 0>, int>);
static_assert(std::is_same_v<field_type_of<Reference, 1>, const int&>);
static_assert(std::is_same_v<field_type_of<Reference, 2>, int>);

// Reference cannot be default constructed
// Not support to get field names of Reference

constexpr int x = 3;
constexpr auto ref = Reference{
    1,
    x,
    4,
};
static_assert(field_of<0>(ref) == 1);
static_assert(field_of<1>(ref) == 3);
static_assert(field_of<2>(ref) == 4);

using namespace magic::details;

int main()
{
    test_for_container();
    std::cout << "All tests passed" << std::endl;
    system("pause");
}
