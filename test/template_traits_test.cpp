#include <gtest/gtest.h>
#include <magic/template_traits.h>

#include <vector>
using namespace magic;

TEST(Template_Traits, None)
{
    using T = std::vector<int>;
    using args = magic::args_type_of<T>;
    EXPECT_TRUE((std::is_same_v<args, std::tuple<identity<int>, identity<std::allocator<int>>>>));
}