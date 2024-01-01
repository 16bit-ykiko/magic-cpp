#include <gtest/gtest.h>
#include <magic/function_traits.h>

using namespace magic;

template <typename T, typename U>
constexpr static bool Same = std::is_same_v<T, U>;

TEST(Function_Traits, None)
{
    using T = int(int, int);
    EXPECT_TRUE((Same<return_type_of<T>, int>));
    EXPECT_TRUE((Same<args_type_of<T>, std::tuple<int, int>>));
    EXPECT_TRUE((Same<primary_type_of<T>, int(int, int)>));
    EXPECT_TRUE((fn_arity_v<T> == 2));
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const)
{
    using T = int(int, int) const;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile)
{
    using T = int(int, int) volatile;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile)
{
    using T = int(int, int) const volatile;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, LRef)
{
    using T = int(int, int)&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_LRef)
{
    using T = int(int, int) const&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile_LRef)
{
    using T = int(int, int, char) volatile&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile_LRef)
{
    using T = int(int, int) const volatile&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, RRef)
{
    using T = int(int, int, char)&&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_RRef)
{
    using T = int(int, int, char) const&&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile_RRef)
{
    using T = int(int, int, char) volatile&&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile_RRef)
{
    using T = int(int, int, char) const volatile&&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Noexcept)
{
    using T = int(int, int) noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Noexcept)
{
    using T = int(int, int) const noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile_Noexcept)
{
    using T = int(int, int) volatile noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile_Noexcept)
{
    using T = int(int, int) const volatile noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, LRef_Noexcept)
{
    using T = int(int, int)& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_LRef_Noexcept)
{
    using T = int(int, int) const& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile_LRef_Noexcept)
{
    using T = int(int, int, char) volatile& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile_LRef_Noexcept)
{
    using T = int(int, int) const volatile& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, RRef_Noexcept)
{
    using T = int(int, int, char)&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_RRef_Noexcept)
{
    using T = int(int, int, char) const&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Volatile_RRef_Noexcept)
{
    using T = int(int, int, char) volatile&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Const_Volatile_RRef_Noexcept)
{
    using T = int(int, int, char) const volatile&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == false));
}

TEST(Function_Traits, Variadic)
{
    using T = int(int, int, char, ...);
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Variadic)
{
    using T = int(int, int, char, ...) const;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_Variadic)
{
    using T = int(int, int, char, ...) volatile;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_Variadic)
{
    using T = int(int, int, char, ...) const volatile;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, LRef_Variadic)
{
    using T = int(int, int, char, ...)&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_LRef_Variadic)
{
    using T = int(int, int, char, ...) const&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_LRef_Variadic)
{
    using T = int(int, int, char, ...) volatile&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_LRef_Variadic)
{
    using T = int(int, int, char, ...) const volatile&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, RRef_Variadic)
{
    using T = int(int, int, char, ...)&&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_RRef_Variadic)
{
    using T = int(int, int, char, ...) const&&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_RRef_Variadic)
{
    using T = int(int, int, char, ...) volatile&&;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_RRef_Variadic)
{
    using T = int(int, int, char, ...) const volatile&&;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == false));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Noexcept_Variadic)
{
    using T = int(int, int, char, ...) noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) volatile noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const volatile noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, LRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...)& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_LRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_LRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) volatile& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_LRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const volatile& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, RRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...)&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_RRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == false));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Volatile_RRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) volatile&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == false));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}

TEST(Function_Traits, Const_Volatile_RRef_Noexcept_Variadic)
{
    using T = int(int, int, char, ...) const volatile&& noexcept;
    EXPECT_TRUE((is_const_fn_v<T> == true));
    EXPECT_TRUE((is_volatile_fn_v<T> == true));
    EXPECT_TRUE((is_lvalue_ref_fn_v<T> == false));
    EXPECT_TRUE((is_rvalue_ref_fn_v<T> == true));
    EXPECT_TRUE((is_noexcept_fn_v<T> == true));
    EXPECT_TRUE((is_c_variadic_fn_v<T> == true));
}
