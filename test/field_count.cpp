#include "magic/struct.h"
#include <utility>

#define FIELD_COUNT_EQUAL(T, N) static_assert(magic::field_count_of<T>() == N)

/**
 * the most important thing is test the precision of the field_count_of
 */

struct A1
{
    int a;
};
FIELD_COUNT_EQUAL(A1, 1);

struct A2
{
    int& a;
};
FIELD_COUNT_EQUAL(A2, 1);

struct A3
{
    int a[2];
};
FIELD_COUNT_EQUAL(A3, 1);

struct A4
{
    int a[2][3];
};
FIELD_COUNT_EQUAL(A4, 1);

struct A5
{
    std::pair<int, int> a;
};
FIELD_COUNT_EQUAL(A5, 1);

struct A6
{
    std::pair<int, int> a[2];
};
FIELD_COUNT_EQUAL(A6, 1);

struct B1
{
    int a;
    int b;
};
FIELD_COUNT_EQUAL(B1, 2);

struct B2
{
    int a;
    int& b;
};
FIELD_COUNT_EQUAL(B2, 2);

struct B3
{
    int& a;
    int b;
};
FIELD_COUNT_EQUAL(B3, 2);

struct B4
{
    int& a;
    int& b;
};
FIELD_COUNT_EQUAL(B4, 2);

struct B5
{
    int a[2];
    int b;
};
FIELD_COUNT_EQUAL(B5, 2);

struct B6
{
    int a;
    int b[2];
};
FIELD_COUNT_EQUAL(B6, 2);

struct B7
{
    int a[2];
    int b[3];
};
FIELD_COUNT_EQUAL(B7, 2);

struct B8
{
    int a[2][3];
    int b[3];
};
FIELD_COUNT_EQUAL(B8, 2);

struct B9
{
    int& a;
    int b[3][4];
};
FIELD_COUNT_EQUAL(B9, 2);

struct B10
{
    int a[2][3];
    int& b;
};
FIELD_COUNT_EQUAL(B10, 2);

struct B11
{
    std::pair<int, int> a;
    int& b;
};
FIELD_COUNT_EQUAL(B11, 2);

struct B12
{
    int& a;
    std::pair<int, int> b;
};
FIELD_COUNT_EQUAL(B12, 2);

struct B13
{
    std::pair<int, int> a;
    std::pair<int, int> b;
};
FIELD_COUNT_EQUAL(B13, 2);

struct B14
{
    std::pair<int, int> a[2];
    std::pair<int, int> b;
};
FIELD_COUNT_EQUAL(B14, 2);

struct B15
{
    std::pair<int, int> a;
    std::pair<int, int> b[2];
};
FIELD_COUNT_EQUAL(B15, 2);

struct B16
{
    std::pair<int, int> a[2];
    std::pair<int, int> b[3];
};
FIELD_COUNT_EQUAL(B16, 2);

struct B17
{
    std::pair<int, int> a[2][3];
    std::pair<int, int> b[3];
};
FIELD_COUNT_EQUAL(B17, 2);

struct C1
{
    int a;
    int b;
    int c;
};
FIELD_COUNT_EQUAL(C1, 3);

struct C2
{
    int a;
    int& b;
    int c;
};
FIELD_COUNT_EQUAL(C2, 3);

struct C3
{
    int& a;
    int b;
    int c;
};
FIELD_COUNT_EQUAL(C3, 3);

struct C4
{
    int& a;
    int& b;
    int c;
};
FIELD_COUNT_EQUAL(C4, 3);

struct C5
{
    int& a;
    int& b;
    int& c;
};
FIELD_COUNT_EQUAL(C5, 3);

struct C6
{
    int a[2];
    int b;
    int c;
};
FIELD_COUNT_EQUAL(C6, 3);

struct C7
{
    int a;
    int b[2];
    int c;
};
FIELD_COUNT_EQUAL(C7, 3);

struct C8
{
    int a;
    int b;
    int c[2];
};
FIELD_COUNT_EQUAL(C8, 3);

struct C9
{
    int a[2];
    int b[3];
    int c;
};
FIELD_COUNT_EQUAL(C9, 3);

struct C10
{
    int a[2];
    int b;
    int c[3];
};
FIELD_COUNT_EQUAL(C10, 3);

struct C11
{
    int a;
    int b[2];
    int c[3];
};
FIELD_COUNT_EQUAL(C11, 3);

struct C12
{
    int a[2];
    int b[3];
    int c[4];
};
FIELD_COUNT_EQUAL(C12, 3);

struct C13
{
    int& a;
    int b[3][4];
    int c;
};
FIELD_COUNT_EQUAL(C13, 3);

struct C14
{
    int a[2][3];
    int& b;
    int c[4];
};
FIELD_COUNT_EQUAL(C14, 3);

struct C15
{
    int a[2][3];
    int b[3];
    int& c;
};
FIELD_COUNT_EQUAL(C15, 3);

struct C16
{
    int& a;
    int b[3];
    int c[4][5];
};
FIELD_COUNT_EQUAL(C16, 3);

struct C17
{
    int a[2];
    int& b;
    int c[4][5];
};
FIELD_COUNT_EQUAL(C17, 3);

struct C18
{
    std::pair<int, int> a;
    int& b;
    int c[4][5];
};
FIELD_COUNT_EQUAL(C18, 3);

struct C19
{
    int& a;
    std::pair<int, int> b;
    int c[4][5];
};
FIELD_COUNT_EQUAL(C19, 3);

struct C20
{
    std::pair<int, int> a;
    std::pair<int, int> b;
    int c[4][5];
};
FIELD_COUNT_EQUAL(C20, 3);

struct C21
{
    std::pair<int, int> a[2];
    std::pair<int, int> b;
    int c[4][5];
};
FIELD_COUNT_EQUAL(C21, 3);

struct LRef
{
    int& a;
};

struct RRef
{
    int&& a;
};

struct CanNotCopy
{
    CanNotCopy(const CanNotCopy&) = delete;
};

struct CanNotMove
{
    CanNotMove(CanNotMove&&) = delete;
};

struct D1
{
    LRef a;
    RRef b;
};
FIELD_COUNT_EQUAL(D1, 2);

struct D2
{
    RRef a;
    LRef b;
    std::pair<int, int> c;
};
FIELD_COUNT_EQUAL(D2, 3);

struct D3
{
    LRef a;
    RRef b;
    std::pair<int, int> c;
    CanNotCopy d;
};
FIELD_COUNT_EQUAL(D3, 4);

struct D4
{
    LRef a;
    RRef b;
    std::pair<int, int> c;
    CanNotMove d;
};
