#include "magic/raw_name.h"
#include <gtest/gtest.h>

using namespace magic;

#define EQUAL(TYPE, name) ASSERT_EQ((raw_name_of<TYPE>()), name) << "test for " #name

TEST(Raw_Name_Of, Fundamental)
{
    EQUAL(int, "int");
    EQUAL(char, "char");
    EQUAL(float, "float");
}

struct A
{
};
class B
{
};

TEST(Raw_Name_Of, Class)
{
    EQUAL(A, "A") << raw_name_of<A>();
    EQUAL(B, "B") << raw_name_of<B>();
}

TEST(Raw_Name_Of, Enum)
{
    enum E
    {
        A,
        B,
        C
    };

    ASSERT_EQ((raw_name_of_enum<E::A>()), "A") << "test for enum";
    ASSERT_EQ((raw_name_of_enum<E::B>()), "B") << "test for enum";
    ASSERT_EQ((raw_name_of_enum<E::C>()), "C") << "test for enum";

    enum class CE
    {
        A,
        B,
        C
    };
    ASSERT_EQ((raw_name_of_enum<CE::A>()), "A") << "test for enum";
    ASSERT_EQ((raw_name_of_enum<CE::B>()), "B") << "test for enum";
    ASSERT_EQ((raw_name_of_enum<CE::C>()), "C") << "test for enum";
}

#ifdef MAGIC_CPP_20_SUPPORT

struct X
{
    int member1;
    int member2;
    int member_3;
};

X x;

TEST(Raw_Name_Of, Member)
{
    ASSERT_EQ((raw_name_of_member<&x.member1>()), "member1");
    ASSERT_EQ((raw_name_of_member<&x.member2>()), "member2");
    ASSERT_EQ((raw_name_of_member<&x.member_3>()), "member_3");
}

#endif