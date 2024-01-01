#ifndef MAGIC_CPP_FIELD_COUNT_H
#define MAGIC_CPP_FIELD_COUNT_H

#include <array>

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
        return []<std::size_t... Is>(std::index_sequence<Is...>) { return requires { T{Any(Is)...}; }; }(std::make_index_sequence<N>{});
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
        if constexpr (try_initialize_with_n<T, N>() && !try_initialize_with_n<T, N + 1>())
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
        if constexpr (max == 0)
        {
            return 0;
        }
        else
        {
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

#endif // MAGIC_CPP_FIELD_COUNT_H