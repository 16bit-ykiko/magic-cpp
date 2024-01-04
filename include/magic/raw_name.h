#ifndef MAGIC_CPP_RAW_NAME_H
#define MAGIC_CPP_RAW_NAME_H

#include "template_traits.h"
#include <string_view>

namespace magic
{
    template <typename T>
    constexpr auto raw_name_of()
    {
        std::string_view name = MAGIC_CPP_FUNCTION_NAME;
#if __GNUC__ || __clang__
        std::size_t start = name.find('=') + 2;
        std::size_t end = name.size() - 1;
        return std::string_view{name.data() + start, end - start};
#elif _MSC_VER
        std::size_t start = name.find('<') + 1;
        std::size_t end = name.rfind(">(");
        name = std::string_view{name.data() + start, end - start};
        start = name.find(' ');
        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 1, name.size() - start - 1};
#else
        static_assert(false, "Unsupported compiler");
#endif
    }

    template <typename T>
    constexpr auto raw_name_of_template()
    {
        std::string_view name = details::template_traits<T>::full_name;
#if __GNUC__ || __clang__
        std::size_t start = name.find('=') + 2;
        std::size_t end = name.size() - 1;
        return std::string_view{name.data() + start, end - start};
#elif _MSC_VER
        std::size_t start = name.find('<') + 1;
        std::size_t end = name.rfind(">(");
        name = std::string_view{name.data() + start, end - start};
        start = name.find(' ');
        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 1, name.size() - start - 1};
#else
        static_assert(false, "Unsupported compiler");
#endif
    }

    template <auto Value>
    constexpr auto raw_name_of()
    {
        std::string_view name = MAGIC_CPP_FUNCTION_NAME;
#if __GNUC__ || __clang__
        std::size_t start = name.find('=') + 2;
        std::size_t end = name.size() - 1;
        return std::string_view{name.data() + start, end - start};
#elif _MSC_VER
        std::size_t start = name.find('<') + 1;
        std::size_t end = name.rfind(">(");
        name = std::string_view{name.data() + start, end - start};
        start = name.find(' ');
        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 1, name.size() - start - 1};
#else
        static_assert(false, "Unsupported compiler");
#endif
    }

    template <auto Value>
    constexpr auto raw_name_of_enum()
    {
        std::string_view name = MAGIC_CPP_FUNCTION_NAME;
#if __GNUC__ || __clang__
        std::size_t start = name.find('=') + 2;
        std::size_t end = name.size() - 1;
        name = std::string_view{name.data() + start, end - start};
        start = name.rfind("::");
        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 2, name.size() - start - 2};
#elif _MSC_VER
        std::size_t start = name.find('<') + 1;
        std::size_t end = name.rfind(">(");
        name = std::string_view{name.data() + start, end - start};
        start = name.rfind("::");
        return start == std::string_view::npos ? name : std::string_view{name.data() + start + 2, name.size() - start - 2};
#else
        static_assert(false, "Unsupported compiler");
#endif
    }

#ifdef MAGIC_CPP_20_SUPPORT
    template <typename T>
    struct Wrapper
    {
        T a;
        constexpr Wrapper(T value) : a(value) {}
    };

    template <Wrapper T>
    constexpr auto raw_name_of_member()
    {
        std::string_view name = MAGIC_CPP_FUNCTION_NAME;
#if __GNUC__ && (!__clang__) && (!_MSC_VER)
        std::size_t start = name.rfind("::") + 2;
        std::size_t end = name.rfind(')');
        return name.substr(start, end - start);
#elif __clang__
        std::size_t start = name.rfind(".") + 1;
        std::size_t end = name.rfind('}');
        return name.substr(start, end - start);
#elif _MSC_VER
        std::size_t start = name.rfind("->") + 2;
        std::size_t end = name.rfind('}');
        return name.substr(start, end - start);
#else
        static_assert(false, "Not supported compiler");
#endif
    }
#endif // MAGIC_CPP_20_SUPPORT
} // namespace magic

#endif // MAGIC_CPP_RAW_NAME_H