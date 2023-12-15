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

static_assert(field_num_of<Point>() == 2);

static_assert(std::is_same_v<field_types_of<Point>, std::tuple<int, int>>);
static_assert(std::is_same_v<field_type_of<Point, 0>, int>);
static_assert(std::is_same_v<field_type_of<Point, 1>, int>);

static_assert(field_names_of<Point>() == std::array<std::string_view, 2>{"x", "y"});
static_assert(field_name_of<Point>(0) == "x");
static_assert(field_name_of<Point>(1) == "y");

constexpr auto point = Point{1, 2};
static_assert(field_of<0>(point) == 1);
static_assert(field_of<1>(point) == 2);

void test_for_point()
{
    Point point = {1, 2};
    auto f = [](auto name, auto&& field)
    {
        if (name == "x")
        {
            assert(field == 1);
        }
        else if (name == "y")
        {
            assert(field == 2);
        }
    };
    magic::foreach (point, f);
}

struct Container
{
    Point point;
    int array[3];
    std::vector<int> vec;
};

static_assert(field_num_of<Container>() == 3);

static_assert(std::is_same_v<field_types_of<Container>, std::tuple<Point, int[3], std::vector<int>>>);
static_assert(std::is_same_v<field_type_of<Container, 0>, Point>);
static_assert(std::is_same_v<field_type_of<Container, 1>, int[3]>);
static_assert(std::is_same_v<field_type_of<Container, 2>, std::vector<int>>);

static_assert(field_names_of<Container>() == std::array<std::string_view, 3>{"point", "array", "vec"});
static_assert(field_name_of<Container>(0) == "point");
static_assert(field_name_of<Container>(1) == "array");
static_assert(field_name_of<Container>(2) == "vec");

void test_for_container()
{
    Container container = {
        {1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    auto f = [](auto name, auto&& field)
    {
        using T = std::remove_cvref_t<decltype(field)>;
        if constexpr (std::is_same_v<T, Point>)
        {
            assert(field.x == 1);
            assert(field.y == 2);
        }
        else if constexpr (std::is_same_v<T, int[3]>)
        {
            assert(field[0] == 3);
            assert(field[1] == 4);
            assert(field[2] == 5);
        }
        else if constexpr (std::is_same_v<T, std::vector<int>>)
        {
            assert(field[0] == 6);
            assert(field[1] == 7);
            assert(field[2] == 8);
        }
    };
    magic::foreach (container, f);
}

// gcc 13 ice, so cannot get the fields number of struct which has reference type member in gcc 13
struct Reference
{
    int x;
    const int& ref;
    int arr[2];
    int y;
};

static_assert(field_num_of<Reference>() == 4);
static_assert(std::is_same_v<field_types_of<Reference>, std::tuple<int, const int&, int[2], int>>);
static_assert(std::is_same_v<field_type_of<Reference, 0>, int>);
static_assert(std::is_same_v<field_type_of<Reference, 1>, const int&>);
static_assert(std::is_same_v<field_type_of<Reference, 2>, int[2]>);
static_assert(std::is_same_v<field_type_of<Reference, 3>, int>);

// Reference cannot be default constructed
// Not support to get field names of Reference

constexpr int x = 3;
constexpr auto ref = Reference{
    1,
    x,
    {2, 3},
    4,
};
static_assert(field_of<0>(ref) == 1);
static_assert(field_of<1>(ref) == 3);
static_assert(field_of<2>(ref)[0] == 2);
static_assert(field_of<2>(ref)[1] == 3);
static_assert(field_of<3>(ref) == 4);

struct Complex
{
    int x;
    std::string ss;
    int arr[3];
    std::vector<int> vec;
    int& a;
};

int main()
{
    test_for_point();
    test_for_container();
    std::cout << "All tests passed" << std::endl;
    system("pause");
}
