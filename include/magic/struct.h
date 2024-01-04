#ifndef MAGIC_CPP_STRUCT_H
#define MAGIC_CPP_STRUCT_H

#include "macro.h"

#ifdef MAGIC_CPP_20_SUPPORT

#include "field_count.h"
#include "raw_name.h"

namespace magic::details
{

    template <typename T>
    constexpr auto field_types_of_impl(T object)
    {
        constexpr auto N = field_count_of<T>();
#include "generate/struct_bind_of_field_types.code"
    }

    template <std::size_t Index>
    constexpr auto&& field_of_impl(auto&& object)
    {
        using T = std::remove_cvref_t<decltype(object)>;
        constexpr auto N = field_count_of<T>();
#include "generate/struct_bind_of_field_access.code"
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
    template <typename T>
    struct Storage
    {
        inline static T value{};
    };

    template <typename T>
    struct Names_Storage
    {
        constexpr static auto value = []<std::size_t... Is>(std::index_sequence<Is...>)
        {
            constexpr auto& object = Storage<T>::value;
            return std::array<std::string_view, sizeof...(Is)>{raw_name_of_member<&field_of_impl<Is>(object)>()...};
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

    template <std::size_t N, typename T>
    struct Field
    {
      private:
        T m_value;

      public:
        constexpr Field(T value) : m_value(std::forward<T>(value)) {}

        using type = field_type_of<std::remove_cvref_t<T>, N>;

        constexpr static std::string_view name() { return field_name_of<std::remove_cvref_t<T>>(N); }

        constexpr static std::string_view type_name() { return raw_name_of<type>(); }

        constexpr static std::size_t index() { return N; }

        constexpr auto&& value() { return field_of<N>(std::forward<T>(m_value)); }

        operator auto &&() { return value(); }

        template <typename Object>
        friend constexpr void foreach (Object&& object, auto&& functor);
    };

    template <typename T>
    concept FIELD = requires(T t) {
        {
            t.name()
        } -> std::same_as<std::string_view>;
        {
            t.type_name()
        } -> std::same_as<std::string_view>;
        {
            t.index()
        } -> std::same_as<std::size_t>;
        {
            t.value()
        };
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
#endif
#endif // MAGIC_CPP_STRUCT_H