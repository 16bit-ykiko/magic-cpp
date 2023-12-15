#ifndef MAGIC_CPP_STRUCT_H
#define MAGIC_CPP_STRUCT_H

#include <array>
#include <iostream>
#include <string_view>
#include <tuple>

// namespace magic::details
// gcc 13 ice
// msvc bug
namespace magic::details
{

    struct Any
    {
        constexpr Any(int) {}

        template <typename T>
        operator T&();

        template <typename T>
        operator T&&();
    };

    template <typename T, std::size_t... Is>
    consteval std::size_t try_initialize_with_n(std::index_sequence<Is...>)
    {
        return requires { T{Any(Is)...}; } && !requires { T{Any(Is)..., Any(0)}; };
    }

    template <typename T, std::size_t N = 0>
    consteval std::size_t total_number_of_fields()
    {
        if constexpr (try_initialize_with_n<T>(std::make_index_sequence<N>{}))
        {
            return N;
        }
        else if constexpr (N >= sizeof(T) * 8 + 1)
        {
            static_assert(N < sizeof(T) * 8 + 1, "Unexpected Error: Cannot get the number of fields");
        }
        else
        {
            return total_number_of_fields<T, N + 1>();
        }
    }

    template <typename T, std::size_t... I1, std::size_t... I2, std::size_t... I3>
    constexpr auto try_place_n_in_pos_helper(std::index_sequence<I1...>, std::index_sequence<I2...>, std::index_sequence<I3...>)
    {
        return requires { T{Any(I1)..., {Any(I2)...}, Any(I3)...}; };
    }

    template <typename T, std::size_t position, std::size_t N, std::size_t Total>
    constexpr auto try_place_n_in_pos()
    {
        if constexpr (N != 0 && position + N < Total)
        {
            return try_place_n_in_pos_helper<T>(std::make_index_sequence<position>{},
                                                std::make_index_sequence<N>{},
                                                std::make_index_sequence<Total - position - N>{});
        }
        else if constexpr (position + N >= Total)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    template <typename T, std::size_t pos, std::size_t Total>
    constexpr auto search_max_in_pos()
    {
        std::size_t result = 0;
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        { ((try_place_n_in_pos<T, pos, Is, Total>() > result ? result = Is : 0), ...); }(std::make_index_sequence<Total>());
        return result;
    }

    template <typename T>
    constexpr auto true_number_of_fields()
    {
        constexpr auto max = total_number_of_fields<T>();
        std::array<std::size_t, max> indices{};
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        { ((indices[Is] = search_max_in_pos<T, Is, max>()), ...); }(std::make_index_sequence<max>());
        std::size_t result = max;
        for (auto n : indices) { (n > 1 ? result -= (n - 1) : 0); }
        return result;
    }
} // namespace magic::details

namespace magic::details
{
    template <typename T>
    struct Id
    {
        using type = T;
    };

    template <typename T>
    constexpr auto get_field_types_impl(T object);

    template <std::size_t Index>
    constexpr auto&& field_of_impl(auto&& object);

    template <typename T>
    constexpr auto field_names_of_impl();
} // namespace magic::details

namespace magic
{
    template <typename T>
    concept Aggregate = std::is_aggregate_v<std::decay_t<T>>;

    /**
     *  @brief Retrieve the number of fields of a struct
     *  @warning cannot get the number of fields of a struct which has reference type member in gcc 13
     *  because the internal error occurs in below occasion
     *  @code
     *  struct Number { operator int&(); };
     *  int& x = { Number{} };
     *
     *  internal compiler error: in reference_binding, at cp/call.cc:2020
     *  @endcode
     *
     */

    template <Aggregate T>
    consteval auto field_num_of()
    {
#if _MSC_VER && !__clang__
        return details::total_number_of_fields<T>();
#else
        return details::true_number_of_fields<T>();
#endif
    }
    template <Aggregate T>
    using field_types_of = typename decltype(details::get_field_types_impl(std::declval<T>()))::type;

    template <Aggregate T, std::size_t Index = 0>
    using field_type_of = std::tuple_element_t<Index, field_types_of<T>>;

    template <std::size_t Index = 0>
    constexpr auto&& field_of(Aggregate auto&& object)
    {
        return details::field_of_impl<Index>(std::forward<decltype(object)>(object));
    }

    template <Aggregate T>
    requires std::is_default_constructible_v<T>
    constexpr auto field_names_of()
    {
        return details::field_names_of_impl<T>();
    }

    template <Aggregate T>
    requires std::is_default_constructible_v<T>
    constexpr std::string_view field_name_of(std::size_t Index)
    {
        return details::field_names_of_impl<T>()[Index];
    }

    constexpr void foreach (Aggregate auto&& object, auto&& functor)
    {
        using T = std::remove_cvref_t<decltype(object)>;
        constexpr auto names = field_names_of<T>();
        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        { (functor(names[Is], field_of<Is>(std::forward<decltype(object)>(object))), ...); }(std::make_index_sequence<field_num_of<T>()>{});
    }

} // namespace magic

namespace magic::details
{
    // used to wrap pointer to data member
    template <typename T>
    struct Wrapper
    {
        T a;
    };

    template <typename T>
    Wrapper(T) -> Wrapper<T>;

    /**
     *  @brief when T is a pointer to data member, return the name of the data member
     *  @code
     *  struct A { int member; };
     *  A a{};
     *  constexpr auto name = main_name_of_pointer<Wrapper{&a.member}>();
     *  // only the pointer to variable of static storage duration time can be used in constant expression
     *  @endcode
     */

    template <auto T>
    constexpr auto main_name_of_pointer()
    {
        constexpr auto is_identifier = [](char c)
        { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; };
#if __GNUC__ && (!__clang__) && (!_MSC_VER)
        std::string_view str = __PRETTY_FUNCTION__;
        std::size_t start = str.rfind("::") + 2;
        std::size_t end = start;
        for (; end < str.size() && is_identifier(str[end]); end++) {}
        return str.substr(start, end - start);
#elif __clang__
        std::string_view str = __PRETTY_FUNCTION__;
        std::size_t start = str.rfind(".") + 1;
        std::size_t end = start;
        for (; end < str.size() && is_identifier(str[end]); end++) {}
        return str.substr(start, end - start);
#elif _MSC_VER
        std::string_view str = __FUNCSIG__;
        std::size_t start = str.rfind("->") + 2;
        std::size_t end = start;
        for (; end < str.size() && is_identifier(str[end]); end++) {}
        return str.substr(start, end - start);
#else
        static_assert(false, "Not supported compiler");
#endif
    }

    template <typename T>
    struct Storage
    {
        inline static T value{};
    };

} // namespace magic::details

namespace magic::details
{
    template <typename T>
    constexpr auto get_field_types_impl(T object)
    {
        constexpr auto N = field_num_of<T>();
        if constexpr (N == 0)
        {
            return Id<std::tuple<>>{};
        }
#include "struct_field_type.ge"
        else
        {
            static_assert(N <= 26, "The maximum number of supported fields is 26.");
        }
    }

    template <std::size_t Index>
    constexpr auto&& field_of_impl(auto&& object)
    {
        using T = std::remove_cvref_t<decltype(object)>;
        constexpr auto N = field_num_of<T>();
        if constexpr (N == 0)
        {
            static_assert(N != 0, "the object has no fields");
        }
#include "struct_field.ge"
        else
        {
            static_assert(N <= 26, "the max of supported fields is 26");
        }
    }

    template <typename T>
    constexpr auto field_names_of_impl()
    {
        constexpr auto N = field_num_of<T>();
        std::array<std::string_view, N> names{};

        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            constexpr auto&& object = Storage<T>::value;
            ((names[Is] = main_name_of_pointer<Wrapper{&field_of_impl<Is>(object)}>()), ...);
        }(std::make_index_sequence<N>{});

        return names;
    }
} // namespace magic::details

#endif // MAGIC_CPP_STRUCT_H