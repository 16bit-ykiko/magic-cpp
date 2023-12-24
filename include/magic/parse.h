#ifndef MAGIC_CPP_MAGIC_PARSE_H
#define MAGIC_CPP_MAGIC_PARSE_H

#include "function_traits.h"
#include "macro.h"
#include "string_view.h"
#include <vector>

namespace magic
{
    template <typename T>
    constexpr auto raw_name_of()
    {
        // clang-format off
        string_view name = MAGIC_CPP_FUNCTION_NAME;
        #if __clang__ || __GNUC__
            std::size_t start = name.find("T = ");
            std::size_t end = name.rfind("]");
            return name.substr(start, end);
        #elif _MSC_VER
            std::size_t start = name.find("name_of<class ");
            start == string_view::npos ? start = name.find("name_of<struct ") : start;
            start == string_view::npos ? start = name.find("<") : start;
            std::size_t end = name.rfind(">(");
            return name.substr(start, end);
        #else
            static_assert(false, "Unsupported compiler");
        #endif
        // clang-format on
    }

    template <auto T>
    constexpr auto raw_name_of()
    {
        // clang-format off
        string_view name = MAGIC_CPP_FUNCTION_NAME;
        #if __clang__ || __GNUC__
            std::size_t start = name.find("T = ");
            std::size_t end = name.rfind("]");
            return name.substr(start, end);
        #elif _MSC_VER
            std::size_t start = name.find("name_of<class ");
            start == string_view::npos ? start = name.find("name_of<struct ") : start;
            start == string_view::npos ? start = name.find("<") : start;
            std::size_t end = name.rfind(">(");
            return name.substr(start, end);
        #else
            static_assert(false, "Unsupported compiler");
        #endif
        // clang-format on
    }

    template <typename T>
    struct type_info;
} // namespace magic

namespace magic::details
{
    enum class TypeKind
    {
        POINTER,
        REFERENCE,
        ARRAY,
        FUNCTION,
        MEMBER,
        TEMPLATE,
        BASIC,
        NTTP
    };

    struct Type
    {
        constexpr virtual ~Type() = default;
        constexpr virtual TypeKind Kind() const = 0;
    };

    struct Pointer : public Type
    {
        Type* pointee;
        string_view modifier;

        constexpr virtual ~Pointer() { delete pointee; }

        constexpr virtual TypeKind Kind() const override { return TypeKind::POINTER; }
    };

    struct Reference : public Type
    {
        Type* pointee;
        string_view modifier;

        constexpr virtual ~Reference() { delete pointee; }

        constexpr virtual TypeKind Kind() const override { return TypeKind::REFERENCE; }
    };

    struct Array : public Type
    {
        Type* element;
        std::size_t size;

        constexpr virtual ~Array() { delete element; }

        constexpr virtual TypeKind Kind() const override { return TypeKind::ARRAY; }
    };

    struct Function : public Type
    {
        Type* return_type;
        std::vector<Type*> parameters;
        string_view modifier;

        constexpr virtual ~Function()
        {
            delete return_type;
            for (auto parameter : parameters) { delete parameter; }
        }

        constexpr virtual TypeKind Kind() const override { return TypeKind::FUNCTION; }
    };

    struct Member : public Type
    {
        Type* class_type;
        Type* member_type;
        string_view modifier;

        constexpr virtual ~Member()
        {
            delete class_type;
            delete member_type;
        }

        constexpr virtual TypeKind Kind() const override { return TypeKind::MEMBER; }
    };

    struct Template : public Type
    {
        string_view name;
        std::vector<Type*> parameters;
        string_view modifier;

        constexpr virtual ~Template()
        {
            for (auto parameter : parameters) { delete parameter; }
        }

        constexpr virtual TypeKind Kind() const override { return TypeKind::TEMPLATE; }
    };

    struct NTTP : public Type
    {
        string_view name;

        constexpr virtual ~NTTP(){};

        constexpr virtual TypeKind Kind() const override { return TypeKind::NTTP; }
    };

    struct BasicType : public Type
    {
        string_view name;
        string_view modifier;

        constexpr virtual ~BasicType(){};

        constexpr virtual TypeKind Kind() const override { return TypeKind::BASIC; }
    };
} // namespace magic::details

namespace magic::details
{
    template <typename T>
    struct type_traits;

    template <typename T>
    constexpr string_view GetCVModifier()
    {
        if constexpr (std::is_const_v<T> && std::is_volatile_v<T>)
        {
            return "[const volatile]";
        }
        else if constexpr (std::is_const_v<T>)
        {
            return "[const]";
        }
        else if constexpr (std::is_volatile_v<T>)
        {
            return "[volatile]";
        }
        else
        {
            return "";
        }
    }

    template <typename T>
    constexpr string_view GetRefModifier()
    {
        if constexpr (std::is_lvalue_reference_v<T>)
        {
            return "[&]";
        }
        else if constexpr (std::is_rvalue_reference_v<T>)
        {
            return "[&&]";
        }
        else
        {
            return "";
        }
    }

    template <typename T>
    constexpr string_view GetFunctionModifier()
    {
        return function_traits<T>::MODIFIER;
    }

    template <typename T>
    constexpr Type* parse(bool is_full_name)
    {
        if constexpr (requires { magic::type_info<T>::name; })
        {
            if (!is_full_name)
            {
                BasicType* result = new BasicType;
                result->name = magic::type_info<T>::name;
                return result;
            }
        }
        else if constexpr (requires { magic::type_info<std::remove_cv_t<T>>::name; })
        {
            if (!is_full_name)
            {
                BasicType* result = new BasicType;
                result->name = magic::type_info<std::remove_cv_t<T>>::name;
                result->modifier = GetCVModifier<T>();
                return result;
            }
        }

        if constexpr (std::is_pointer_v<T>)
        {
            Pointer* result = new Pointer;
            result->pointee = parse<std::remove_pointer_t<T>>(is_full_name);
            result->modifier = GetCVModifier<T>();
            return result;
        }
        else if constexpr (std::is_reference_v<T>)
        {
            Reference* result = new Reference;
            result->pointee = parse<std::remove_reference_t<T>>(is_full_name);
            result->modifier = GetRefModifier<T>();
            return result;
        }
        else if constexpr (std::is_array_v<T>)
        {
            Array* result = new Array;
            result->element = parse<std::remove_extent_t<T>>(is_full_name);
            result->size = std::extent_v<T>;
            return result;
        }
        else if constexpr (std::is_function_v<T>)
        {
            auto modifier = GetFunctionModifier<T>();
            using F = typename function_traits<T>::type;
            return type_traits<F>::parse(modifier, is_full_name);
        }
        else if constexpr (requires { type_traits<std::remove_cv_t<T>>::parse; })
        {
            auto modifier = GetCVModifier<T>();
            return type_traits<std::remove_cv_t<T>>::parse(modifier, is_full_name);
        }
        else
        {
            BasicType* result = new BasicType;
            result->name = raw_name_of<std::remove_cv_t<T>>();
            result->modifier = GetCVModifier<T>();
            return result;
        }
    }

} // namespace magic::details

namespace magic::details
{
    template <typename R, typename... Args>
    struct type_traits<R(Args...)>
    {
        constexpr static Type* parse(string_view modifier, bool is_full_name)
        {
            Function* result = new Function;
            result->return_type = magic::details::parse<R>(is_full_name);
            result->modifier = modifier;
            ([&]<typename Type> { result->parameters.push_back(magic::details::parse<Type>(is_full_name)); }.template operator()<Args>(),
             ...);
            return result;
        }
    };

    template <typename M, typename C>
    struct type_traits<M C::*>
    {
        constexpr static Type* parse(string_view modifier, bool is_full_name)
        {
            Member* result = new Member;
            result->class_type = magic::details::parse<C>(is_full_name);
            result->member_type = magic::details::parse<M>(is_full_name);
            result->modifier = modifier;
            return result;
        }
    };
} // namespace magic::details

/**
 * The code below is generated by script/parse_template.py
 * to enumerate all possible combinations of template parameters for value and type.
 * The maximum number of template parameters is 4.
 */

namespace magic::details
{
#define MAGIC_PARSE_START                                                                                                                  \
    constexpr static Type* parse(string_view modifier, bool is_full_name)                                                                  \
    {                                                                                                                                      \
        Template* result = new Template;                                                                                                   \
        result->name = name;                                                                                                               \
        result->modifier = modifier;

#define MAGIC_PARSE_END                                                                                                                    \
    return result;                                                                                                                         \
    }

#define MAGIC_ADD_TYPE(T) result->parameters.push_back(magic::details::parse<T>(is_full_name));

#define MAGIC_ADD_NTTP(T)                                                                                                                  \
    NTTP* nttp##T = new NTTP();                                                                                                            \
    nttp##T->name = magic::raw_name_of<T>();                                                                                               \
    result->parameters.push_back(nttp##T);

#define MAGIC_ADD_VARADIC_TYPES(Ts)                                                                                                        \
    ([&]<typename TYPE> { result->parameters.push_back(magic::details::parse<TYPE>(is_full_name)); }.template operator()<Ts>(), ...);

#define MAGIC_ADD_VARADIC_NTTPS(Ts)                                                                                                        \
    (                                                                                                                                      \
        [&]<auto VALUE>                                                                                                                    \
        {                                                                                                                                  \
            NTTP* nttps = new NTTP();                                                                                                      \
            nttps->name = magic::raw_name_of<VALUE>();                                                                                     \
            result->parameters.push_back(nttps);                                                                                           \
        }.template operator()<Ts>(),                                                                                                       \
        ...);

#if __clang__ || __GNUC__
#define MAGIC_TEMPLATE_NAME                                                                                                                \
    constexpr static string_view name = []                                                                                                 \
    {                                                                                                                                      \
        string_view name = MAGIC_CPP_FUNCTION_NAME;                                                                                        \
        auto first = name.find("type_traits<");                                                                                            \
        auto end = first;                                                                                                                  \
        for (; end < name.size() && name[end] != '<'; ++end) {}                                                                            \
        return name.substr(first, end);                                                                                                    \
    }();

#elif _MSC_VER
#define MAGIC_TEMPLATE_NAME                                                                                                                \
    constexpr static auto name = []                                                                                                        \
    {                                                                                                                                      \
        string_view name = MAGIC_CPP_FUNCTION_NAME;                                                                                        \
        auto first = name.find("type_traits<class ");                                                                                      \
        first == string_view::npos ? first = name.find("type_traits<struct") : first;                                                      \
        auto end = first;                                                                                                                  \
        for (; end < name.size() && name[end] != '<'; ++end) {}                                                                            \
        return name.substr(first, end);                                                                                                    \
    }();

#else
    static_assert(false, "Not supported compiler");
#endif

#include "generate/template_parse.code"

#undef MAGIC_PARSE_START
#undef MAGIC_PARSE_END
#undef MAGIC_ADD_TYPE
#undef MAGIC_ADD_NTTP
#undef MAGIC_ADD_VARADIC_TYPES
#undef MAGIC_ADD_VARADIC_NTTPS
#undef MAGIC_TEMPLATE_NAME
} // namespace magic::details

#endif // MAGIC_CPP_MAGIC_PARSE_H