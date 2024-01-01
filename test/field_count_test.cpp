#include <gtest/gtest.h>
#include <magic/struct.h>

using namespace magic;

TEST(Field_Count, NoRef_NoArray)
{
    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    ASSERT_EQ(field_count_of<Vec3>(), 3);

    struct Line
    {
        Vec3 start;
        Vec3 end;
    };

    ASSERT_EQ(field_count_of<Line>(), 2);

    struct PairPair
    {
        std::pair<int, int> pair1;
        std::pair<int, int> pair2;
    };

    ASSERT_EQ(field_count_of<PairPair>(), 2);
}

TEST(Field_Count, Ref)
{
    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    ASSERT_EQ(field_count_of<Vec3>(), 3);

    struct LRef
    {
        Vec3& start;
    };

    ASSERT_EQ(field_count_of<LRef>(), 1);

    struct RRef
    {
        Vec3&& start;
    };

    ASSERT_EQ(field_count_of<RRef>(), 1);

    struct ConstRef
    {
        const Vec3& start;
    };

    ASSERT_EQ(field_count_of<ConstRef>(), 1);

    struct LRRef
    {
        Vec3& start;
        Vec3&& end;
    };
    ASSERT_EQ(field_count_of<LRRef>(), 2);

    struct LConstRef
    {
        Vec3& start;
        const Vec3& end;
    };
    ASSERT_EQ(field_count_of<LConstRef>(), 2);

    struct RConstRef
    {
        Vec3&& start;
        const Vec3& end;
    };
    ASSERT_EQ(field_count_of<RConstRef>(), 2);

    struct LRConstRef
    {
        Vec3& start;
        Vec3&& end;
        const Vec3& end2;
    };

    ASSERT_EQ(field_count_of<LRConstRef>(), 3);

    struct ConstLRRef
    {
        const Vec3& start;
        Vec3& end;
        Vec3&& end2;
    };

    ASSERT_EQ(field_count_of<ConstLRRef>(), 3);

    struct VLRConstRef
    {
        Vec3 start;
        Vec3& end;
        Vec3&& end2;
        const Vec3& end3;
    };

    ASSERT_EQ(field_count_of<VLRConstRef>(), 4);
}

TEST(Field_Count, Array)
{
    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    ASSERT_EQ(field_count_of<Vec3>(), 3);

    struct Vec3Array
    {
        Vec3 array[3];
    };

    ASSERT_EQ(field_count_of<Vec3Array>(), 1);

    struct Vec3Array2
    {
        Vec3 array[2][3];
    };

    ASSERT_EQ(field_count_of<Vec3Array2>(), 1);

    struct Vec3Array3
    {
        Vec3 array[3];
        Vec3 array2[2];
        Vec3 point;
    };

    ASSERT_EQ(field_count_of<Vec3Array3>(), 3);
}