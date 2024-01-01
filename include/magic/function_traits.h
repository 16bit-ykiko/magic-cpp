#ifndef MAGIC_CPP_FUNCTION_TRAITS
#define MAGIC_CPP_FUNCTION_TRAITS

#include <tuple>

namespace magic::details
{
    template <typename T>
    struct function_traits
    {
        constexpr static bool is_const = false;
        constexpr static bool is_volatile = false;
        constexpr static bool is_lvalue_ref = false;
        constexpr static bool is_rvalue_ref = false;
        constexpr static bool is_noexcept = false;
        constexpr static bool is_c_variadic = false;
    };

#define MAGIC_CPP_FUNCTION_TRAITS_IMPL(modifier, const, volatile, lref, rref, noexcept)                                                    \
    template <typename R, typename... Args>                                                                                                \
    struct function_traits<R(Args...) modifier>                                                                                            \
    {                                                                                                                                      \
        using return_type = R;                                                                                                             \
        using args_type = std::tuple<Args...>;                                                                                             \
        using primary_type = R(Args...);                                                                                                   \
        constexpr static bool is_const = const;                                                                                            \
        constexpr static bool is_volatile = volatile;                                                                                      \
        constexpr static bool is_lvalue_ref = lref;                                                                                        \
        constexpr static bool is_rvalue_ref = rref;                                                                                        \
        constexpr static bool is_noexcept = noexcept;                                                                                      \
        constexpr static bool is_c_variadic = false;                                                                                       \
    };

#define MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(modifier, const, volatile, lref, rref, noexcept)                                           \
    template <typename R, typename... Args>                                                                                                \
    struct function_traits<R(Args..., ...) modifier>                                                                                       \
    {                                                                                                                                      \
        using return_type = R;                                                                                                             \
        using args_type = std::tuple<Args...>;                                                                                             \
        using primary_type = R(Args...);                                                                                                   \
        constexpr static bool is_const = const;                                                                                            \
        constexpr static bool is_volatile = volatile;                                                                                      \
        constexpr static bool is_lvalue_ref = lref;                                                                                        \
        constexpr static bool is_rvalue_ref = rref;                                                                                        \
        constexpr static bool is_noexcept = noexcept;                                                                                      \
        constexpr static bool is_c_variadic = true;                                                                                        \
    };

    MAGIC_CPP_FUNCTION_TRAITS_IMPL(, false, false, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const, true, false, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile, false, true, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile, true, true, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(&, false, false, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const&, true, false, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile&, false, true, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile&, true, true, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(&&, false, false, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const&&, true, false, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile&&, false, true, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile&&, true, true, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(noexcept, false, false, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const noexcept, true, false, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile noexcept, false, true, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile noexcept, true, true, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(& noexcept, false, false, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const& noexcept, true, false, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile& noexcept, false, true, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile& noexcept, true, true, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(&& noexcept, false, false, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const&& noexcept, true, false, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(volatile&& noexcept, false, true, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_IMPL(const volatile&& noexcept, true, true, false, true, true)

    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(, false, false, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const, true, false, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile, false, true, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile, true, true, false, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(&, false, false, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const&, true, false, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile&, false, true, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile&, true, true, true, false, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(&&, false, false, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const&&, true, false, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile&&, false, true, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile&&, true, true, false, true, false)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(noexcept, false, false, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const noexcept, true, false, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile noexcept, false, true, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile noexcept, true, true, false, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(& noexcept, false, false, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const& noexcept, true, false, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile& noexcept, false, true, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile& noexcept, true, true, true, false, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(&& noexcept, false, false, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const&& noexcept, true, false, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(volatile&& noexcept, false, true, false, true, true)
    MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL(const volatile&& noexcept, true, true, false, true, true)

#undef MAGIC_CPP_FUNCTION_TRAITS_IMPL
#undef MAGIC_CPP_FUNCTION_TRAITS_VARIADIC_IMPL

    template <typename T, typename U>
    struct args_type_of_impl;

    template <typename T>
    struct args_type_of_impl<T, std::enable_if_t<std::is_function_v<T>>>
    {
        using type = typename function_traits<T>::args_type;
    };
} // namespace magic::details

namespace magic
{
    template <typename T>
    using return_type_of = typename details::function_traits<T>::return_type;

    template <typename T>
    using args_type_of = typename details::args_type_of_impl<T, void>::type;

    template <typename T>
    using first_arg_type_of = std::tuple_element_t<0, args_type_of<T>>;

    template <typename T>
    using second_arg_type_of = std::tuple_element_t<1, args_type_of<T>>;

    template <typename T>
    using third_arg_type_of = std::tuple_element_t<2, args_type_of<T>>;

    template <typename T>
    using primary_type_of = typename details::function_traits<T>::primary_type;

    template <typename T>
    constexpr static std::size_t fn_arity_v = std::tuple_size_v<args_type_of<T>>;

    template <typename T>
    constexpr static bool is_const_fn_v = details::function_traits<T>::is_const;

    template <typename T>
    constexpr static bool is_volatile_fn_v = details::function_traits<T>::is_volatile;

    template <typename T>
    constexpr static bool is_lvalue_ref_fn_v = details::function_traits<T>::is_lvalue_ref;

    template <typename T>
    constexpr static bool is_rvalue_ref_fn_v = details::function_traits<T>::is_rvalue_ref;

    template <typename T>
    constexpr static bool is_noexcept_fn_v = details::function_traits<T>::is_noexcept;

    template <typename T>
    constexpr static bool is_c_variadic_fn_v = details::function_traits<T>::is_c_variadic;
} // namespace magic

#endif // MAGIC_CPP_FUNCTION_TRAITS
