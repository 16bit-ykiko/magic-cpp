#ifndef MAGIC_CPP_STRUCT_H
#define MAGIC_CPP_STRUCT_H

#include "magic/parse.h"
#include <array>
#include <string_view>
#include <tuple>
#include <type_traits>

// gcc 13 ice
// msvc bug
// namespace magic::details
namespace magic::details
{
    struct Any
    {
        constexpr Any(int) {}

        template <typename T>
        requires std::is_copy_constructible_v<T>
        operator T&();

        template <typename T>
        requires std::is_move_constructible_v<T>
        operator T&&();

        template <typename T>
        requires(!std::is_copy_constructible_v<T> && !std::is_move_constructible_v<T>)
        operator T();
    };

    template <typename T, std::size_t N>
    consteval std::size_t try_initialize_with_n()
    {
        return []<std::size_t... Is>(std::index_sequence<Is...>) {
            return requires { T{Any(Is)...}; } && !requires { T{Any(Is)..., Any(0)}; };
        }(std::make_index_sequence<N>{});
    }

    template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
    consteval std::size_t try_initialize_with_three_parts()
    {
        return []<std::size_t... I1, std::size_t... I2, std::size_t... I3>(std::index_sequence<I1...>,
                                                                           std::index_sequence<I2...>,
                                                                           std::index_sequence<I3...>) {
            return requires { T{Any(I1)..., {Any(I2)...}, Any(I3)...}; };
        }(std::make_index_sequence<N1>{}, std::make_index_sequence<N2>{}, std::make_index_sequence<N3>{});
    }

    template <typename T, std::size_t N = 0>
    consteval std::size_t total_count_of_fields()
    {
        if constexpr (N > sizeof(T) * 8 + 1)
        {
            static_assert(N <= sizeof(T) * 8 + 1, "Unexpected Error in total_count_of_fields");
        }
        else if constexpr (try_initialize_with_n<T, N>())
        {
            return N;
        }
        else
        {
            return total_count_of_fields<T, N + 1>();
        }
    }

    template <typename T, std::size_t position, std::size_t N>
    constexpr bool try_place_n_in_pos()
    {
        constexpr auto Total = total_count_of_fields<T>();
        if constexpr (N == 0)
        {
            return true;
        }
        else if constexpr (position + N <= Total)
        {
            return try_initialize_with_three_parts<T, position, N, Total - position - N>();
        }
        else
        {
            return false;
        }
    }

    template <typename T, std::size_t pos, std::size_t N = 0, std::size_t Max = 10>
    constexpr bool has_extra_elements()
    {
        constexpr auto Total = total_count_of_fields<T>();
        if constexpr (try_initialize_with_three_parts<T, pos, N, Total - pos - 1>())
        {
            return false;
        }
        else if constexpr (N + 1 <= Max)
        {
            return has_extra_elements<T, pos, N + 1>();
        }
        else
        {
            return true;
        }
    }

    template <typename T, std::size_t pos>
    constexpr std::size_t search_max_in_pos()
    {
        constexpr auto Total = total_count_of_fields<T>();
        if constexpr (!has_extra_elements<T, pos>())
        {
            return 1;
        }
        else
        {
            std::size_t result = 0;
            [&]<std::size_t... Is>(std::index_sequence<Is...>)
            { ((try_place_n_in_pos<T, pos, Is>() ? result = Is : 0), ...); }(std::make_index_sequence<Total + 1>());
            return result;
        }
    }

    template <typename T, std::size_t N = 0>
    constexpr auto search_all_extra_index(auto&& array)
    {
        constexpr auto total = total_count_of_fields<T>();
        constexpr auto value = std::max<std::size_t>(search_max_in_pos<T, N>(), 1);
        array[N] = value;
        if constexpr (N + value < total)
        {
            search_all_extra_index<T, N + value>(array);
        }
    }

    template <typename T>
    constexpr auto true_count_of_fields()
    {
        constexpr auto max = total_count_of_fields<T>();
        std::array<std::size_t, max> indices = {1};
        search_all_extra_index<T>(indices);
        std::size_t result = max;
        std::size_t index = 0;
        while (index < max)
        {
            auto n = indices[index];
            result -= n - 1;
            index += n;
        }
        return result;
    }
} // namespace magic::details
namespace magic
{
    template <typename T>
    struct type_info;

    /**
     *  @brief Retrieve the count of fields of a struct
     *  @warning cannot get the count of fields of a struct which has reference
     * type member in gcc 13 because the internal error occurs in below occasion
     *  @code
     *  struct Number { operator int&(); };
     *  int& x = { Number{} };
     *
     *  internal compiler error: in reference_binding, at cp/call.cc:2020
     *  @endcode
     *
     */
    template <typename T>
    requires std::is_aggregate_v<T>
    consteval auto field_count_of()
    {
        if constexpr (requires { type_info<T>::count; })
        {
            return type_info<T>::count;
        }
        else
        {
            return details::true_count_of_fields<T>();
        }
    }
} // namespace magic

namespace magic::details
{
    template <typename T>
    constexpr auto field_types_of_impl(T object)
    {
        constexpr auto N = field_count_of<T>();
// clang-format off
        #include "struct_bind_of_field_types.ge"
        // clang-format on
    }

    template <std::size_t Index>
    constexpr auto&& field_of_impl(auto&& object)
    {
        using T = std::remove_cvref_t<decltype(object)>;
        constexpr auto N = field_count_of<T>();
// clang-format off
        #include "struct_bind_of_field_access.ge"
        // clang-format on
    }
} // namespace magic::details

namespace magic
{
    template <typename T>
    requires std::is_aggregate_v<T>
    using field_types_of = typename decltype(details::field_types_of_impl(std::declval<T>()))::type;

    template <typename T, std::size_t Index = 0>
    requires std::is_aggregate_v<T>
    using field_type_of = std::tuple_element_t<Index, field_types_of<T>>;

    template <std::size_t Index = 0>
    constexpr auto&& field_of(auto&& object) requires std::is_aggregate_v<std::remove_cvref_t<decltype(object)>>
    {
        return details::field_of_impl<Index>(std::forward<decltype(object)>(object));
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
     *  @brief when T is a pointer to data member, return the name of the data
     * member
     *  @code
     *  struct A { int member; };
     *  A a{};
     *  constexpr auto name = main_name_of_pointer<Wrapper{&a.member}>();
     *  // only the pointer to variable of static storage duration time can be used
     * in constant expression
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

    template <typename T>
    struct Names_Storage
    {
        constexpr static auto value = []<std::size_t... Is>(std::index_sequence<Is...>) {
            return std::array<std::string_view, sizeof...(Is)>{main_name_of_pointer<Wrapper{&field_of_impl<Is>(Storage<T>::value)}>()...};
        }(std::make_index_sequence<field_count_of<T>()>{});
    };

} // namespace magic::details

namespace magic
{
    template <typename T>
    requires(std::is_aggregate_v<T> && std::is_default_constructible_v<T>)
    constexpr auto& field_names_of()
    {
        return details::Names_Storage<T>::value;
    }

    template <typename T>
    requires(std::is_aggregate_v<T> && std::is_default_constructible_v<T>)
    constexpr std::string_view field_name_of(std::size_t Index)
    {
        return field_names_of<T>()[Index];
    }

    template <typename T>
    constexpr std::string_view name_of();

    template <std::size_t N, typename T>
    struct Field
    {
      private:
        T m_value;

      public:
        constexpr Field(T value) : m_value(std::forward<T>(value)) {}

        using type = field_type_of<std::remove_cvref_t<T>, N>;

        constexpr static std::string_view name() { return field_name_of<std::remove_cvref_t<T>>(N); }

        constexpr static std::string_view type_name() { return name_of<type>(); }

        constexpr static std::size_t index() { return N; }

        constexpr auto&& value() { return field_of<N>(std::forward<T>(m_value)); }

        operator auto &&() { return value(); }

        template <typename Object>
        friend constexpr void foreach (Object&& object, auto&& functor);
    };

    template <typename Object>
    constexpr void foreach (Object&& object, auto&& functor)
    {
        using T = std::remove_cvref_t<decltype(object)>;
        constexpr auto condition = std::is_aggregate_v<T> && std::is_default_constructible_v<T>;
        static_assert(condition, "T must be an aggregate type and default constructible");
        if constexpr (condition)
        {
            [&]<std::size_t... Is>(std::index_sequence<Is...>)
            { (functor(Field<Is, Object>{std::forward<Object>(object)}), ...); }(std::make_index_sequence<field_count_of<T>()>{});
        }
    }
} // namespace magic

#endif // MAGIC_CPP_STRUCT_H