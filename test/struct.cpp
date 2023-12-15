#include <cassert>
#include <iostream>
#include <magic/struct.h>
#include <type_traits>
#include <vector>

using namespace magic;

/**
 * the most important thing is test the precision of the field_num_of
 */
#define FIELD_NUM_EQUAL(T, N) static_assert(field_num_of<T>() == N)

struct Point
{
    int x;
    int y;
};

struct A
{
    Point a;
};

FIELD_NUM_EQUAL(A, 1);

struct B
{
    Point& a;
};

FIELD_NUM_EQUAL(B, 1);

struct C
{
    Point a[2];
};

FIELD_NUM_EQUAL(C, 1);

struct D
{
    Point a[2][3];
};

FIELD_NUM_EQUAL(D, 1);

struct E
{
    Point a[2];
    Point b[2][2];
};
FIELD_NUM_EQUAL(E, 2);

struct F
{
    Point a[2];
    Point b[2][2];
    Point& c;
};

FIELD_NUM_EQUAL(F, 3);

struct G
{
    Point a[2];
    Point b[2][2];
    Point& c;
    Point d[2];
};

FIELD_NUM_EQUAL(G, 4);

struct H
{
    Point a[2];
    Point b[2][2];
    Point c;
    Point d[2];
    Point& e;
};

FIELD_NUM_EQUAL(H, 5);

/**
 * test for other functions
 */

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
    foreach(container, f);
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

using namespace magic::details;

int main()
{
    test_for_container();
    std::cout << "All tests passed" << std::endl;
    system("pause");
}
