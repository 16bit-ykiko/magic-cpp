#ifndef MAGIC_CPP_TEMPLATE_TRAITS_H
#define MAGIC_CPP_TEMPLATE_TRAITS_H

#include "macro.h"
#include <tuple>

namespace magic
{
    template <typename T, T... Value>
    struct identity
    {
        using type = T;
        constexpr static bool has_value = false;
    };

    template <typename T, T Value>
    struct identity<T, Value>
    {
        using type = T;
        constexpr static T value = Value;
        constexpr static bool has_value = true;
    };
} // namespace magic

namespace magic::details
{
    // clang-format off
    #include "generate/template.code"
    // clang-format on

    template <typename T>
    using template_traits = decltype(details::template_traits_impl<T>());

    template <typename T, typename U>
    struct args_type_of_impl;

    template <typename T>
    struct args_type_of_impl<T, std::void_t<std::enable_if_t<is_template<T>(), void>>>
    {
        using type = typename template_traits<T>::args_type;
    };

} // namespace magic::details

namespace magic
{
    template <typename T>
    constexpr static bool is_template_class_v = details::is_template<T>();

    template <typename T>
    using args_type_of = typename details::args_type_of_impl<T, void>::type;

    template <typename T>
    constexpr static std::size_t template_arity_v = std::tuple_size_v<args_type_of<T>>;

} // namespace magic

#endif // MAGIC_CPP_TEMPLATE_TRAITS_H