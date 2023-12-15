#ifndef MAGIC_CPP_FUNCTION_TRAITS_H
#define MAGIC_CPP_FUNCTION_TRAITS_H

namespace magic::details
{
    template <typename T>
    struct function_traits;

    template <typename R, typename... Args>
    struct function_traits<R(Args...)>
    {
        constexpr static auto MODIFIER = "";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const>
    {
        constexpr static auto MODIFIER = "[const]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile>
    {
        constexpr static auto MODIFIER = "[volatile]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile>
    {
        constexpr static auto MODIFIER = "[const volatile]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...)&>
    {
        constexpr static auto MODIFIER = "[&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const&>
    {
        constexpr static auto MODIFIER = "[const &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile&>
    {
        constexpr static auto MODIFIER = "[volatile &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile&>
    {
        constexpr static auto MODIFIER = "[const volatile &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) &&>
    {
        constexpr static auto MODIFIER = "[&&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const&&>
    {
        constexpr static auto MODIFIER = "[const &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile&&>
    {
        constexpr static auto MODIFIER = "[volatile &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile&&>
    {
        constexpr static auto MODIFIER = "[const volatile &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) noexcept>
    {
        constexpr static auto MODIFIER = "[noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const noexcept>
    {
        constexpr static auto MODIFIER = "[const noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile noexcept>
    {
        constexpr static auto MODIFIER = "[volatile noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile noexcept>
    {
        constexpr static auto MODIFIER = "[const volatile noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) & noexcept>
    {
        constexpr static auto MODIFIER = "[& noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const & noexcept>
    {
        constexpr static auto MODIFIER = "[const & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile & noexcept>
    {
        constexpr static auto MODIFIER = "[volatile & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile & noexcept>
    {
        constexpr static auto MODIFIER = "[const volatile & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) && noexcept>
    {
        constexpr static auto MODIFIER = "[&& noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const && noexcept>
    {
        constexpr static auto MODIFIER = "[const && noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) volatile && noexcept>
    {
        constexpr static auto MODIFIER = "[volatile && noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args...) const volatile && noexcept>
    {
        constexpr static auto MODIFIER = "[const volatile && noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...)>
    {
        constexpr static auto MODIFIER = "[...]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const>
    {
        constexpr static auto MODIFIER = "[...] [const]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile>
    {
        constexpr static auto MODIFIER = "[...] [volatile]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile>
    {
        constexpr static auto MODIFIER = "[...] [const volatile]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...)&>
    {
        constexpr static auto MODIFIER = "[...] [&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const&>
    {
        constexpr static auto MODIFIER = "[...] [const &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile&>
    {
        constexpr static auto MODIFIER = "[...] [volatile &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile&>
    {
        constexpr static auto MODIFIER = "[...] [const volatile &]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) &&>
    {
        constexpr static auto MODIFIER = "[...] [&&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const&&>
    {
        constexpr static auto MODIFIER = "[...] [const &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile&&>
    {
        constexpr static auto MODIFIER = "[...] [volatile &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile&&>
    {
        constexpr static auto MODIFIER = "[...] [const volatile &&]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) noexcept>
    {
        constexpr static auto MODIFIER = "[...] [noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile noexcept>
    {
        constexpr static auto MODIFIER = "[...] [volatile noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const volatile noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) & noexcept>
    {
        constexpr static auto MODIFIER = "[...] [& noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const & noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile & noexcept>
    {
        constexpr static auto MODIFIER = "[...] [volatile & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile & noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const volatile & noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) && noexcept>
    {
        constexpr static auto MODIFIER = "[...] [&& noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const && noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const && noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) volatile && noexcept>
    {
        constexpr static auto MODIFIER = "[...] [volatile && noexcept]";
        using type = R(Args...);
    };

    template <typename R, typename... Args>
    struct function_traits<R(Args..., ...) const volatile && noexcept>
    {
        constexpr static auto MODIFIER = "[...] [const volatile && noexcept]";
        using type = R(Args...);
    };

} // namespace magic::details
#endif