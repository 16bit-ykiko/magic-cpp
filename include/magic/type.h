#ifndef MAGIC_CPP_TYPE_H
#define MAGIC_CPP_TYPE_H
#include "parse.h"
#include <algorithm>
#include <array>

#define NOCOLOR 0
#define MODIFIER_COLOR 0x98C379
#define TYPE_COLOR 0xE5C07B
#define BUILTIN_COLOR 0xC678DD
#define NTTP_COLOR 0xD19A66
#define TAG_COLOR 0x5C6370

namespace magic
{
    class String : public std::vector<char>
    {

      public:
        constexpr String() = default;

        constexpr String(const char* str) : String(std::string_view(str)) {}

        constexpr String(std::string_view str)
        {
            for (std::size_t i = 0; i < str.size(); i++) { push_back(str[i]); }
        }

        constexpr String(const String& other) = default;

        constexpr String(String&& other) = default;

        constexpr String& operator=(String&& other) = default;

        constexpr ~String() = default;

        friend constexpr String operator+(const String& LHS, const String& RHS)
        {
            String result;
            result.insert(result.end(), LHS.begin(), LHS.end());
            result.insert(result.end(), RHS.begin(), RHS.end());
            return result;
        }

        constexpr String& operator+=(const String& other)
        {
            *this = *this + other;
            return *this;
        }

        constexpr static String from(std::size_t N)
        {
            if (N == 0)
            {
                return "0";
            }
            String result = "";
            while (N > 0)
            {
                char digit = '0' + N % 10;
                result.insert(result.begin(), digit);
                N /= 10;
            }
            return result;
        }

        operator std::string_view() const { return std::string_view(data(), size()); }

        friend auto& operator<<(std::ostream& os, const String& str)
        {
            os << std::string_view(str);
            return os;
        }

        constexpr String coloring(uint32_t color) const
        {
            if constexpr (NOCOLOR)
            {
                return *this;
            }
            if (this->size() == 0)
            {
                return {};
            }
            uint8_t R = (color >> 16) & 0xFF;
            uint8_t G = (color >> 8) & 0xFF;
            uint8_t B = color & 0xFF;
            return "\033[38;2;" + String::from(R) + ";" + String::from(G) + ";" + String::from(B) + "m" + *this + "\033[0m";
        }
    };

    constexpr auto format(const std::string_view fmt, auto... args)
    {
        constexpr std::size_t N = sizeof...(args);
        std::array<std::string_view, N> args_ = {args...};
        String result;
        std::size_t index = 0;
        for (std::size_t i = 0; i < fmt.size(); i++)
        {
            if (fmt[i] == '{')
            {
                std::size_t j = i + 1;
                while (j < fmt.size() && fmt[j] != '}') { j += 1; }
                if (j == fmt.size())
                {
                    throw;
                }
                result += args_[index];
                index += 1;
                i = j;
            }
            else
            {
                result.push_back(fmt[i]);
            }
        }
        return result;
    }

} // namespace magic

namespace magic::details
{
    constexpr static String coloring(String text, uint32_t color)
    {
        if constexpr (NOCOLOR)
        {
            return text;
        }
        if (text.size() == 0)
        {
            return text;
        }
        uint8_t R = (color >> 16) & 0xFF;
        uint8_t G = (color >> 8) & 0xFF;
        uint8_t B = color & 0xFF;
        return "\033[38;2;" + String::from(R) + ";" + String::from(G) + ";" + String::from(B) + "m" + text + "\033[0m";
    }

    /**
     * used to tree_of_impl the type tree and output the final string
     */

    constexpr void tree_of_impl(Type* type, String& result, String prefix = "", bool is_last = true, String tag = "")
    {
        String new_prefix = prefix + (is_last ? "    " : "│   ");
        for (std::size_t i = 0; i < tag.size(); i++) { new_prefix.push_back(' '); }
        String branch = prefix + (is_last ? "└── " : "├── ") + coloring(tag, TAG_COLOR);

        if (type->Kind() == TypeKind::BASIC)
        {
            auto basic = static_cast<BasicType*>(type);
            auto name = String(basic->name).coloring(TYPE_COLOR);
            auto modifier = String(basic->modifier).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            auto pointer = static_cast<Pointer*>(type);
            auto name = String("ptr").coloring(BUILTIN_COLOR);
            auto modifier = String(pointer->modifier).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
            tree_of_impl(pointer->pointee, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            auto ref = static_cast<Reference*>(type);
            auto name = String("ref").coloring(BUILTIN_COLOR);
            auto modifier = String(ref->modifier).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
            tree_of_impl(ref->pointee, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            auto array = static_cast<Array*>(type);
            auto name = String("array").coloring(BUILTIN_COLOR);
            auto modifier = format("[N = {}]", String::from(array->size)).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
            tree_of_impl(array->element, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            auto function = static_cast<Function*>(type);
            auto name = String("function").coloring(BUILTIN_COLOR);
            auto modifier = String(function->modifier).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
            tree_of_impl(function->return_type, result, new_prefix, false, "R: ");
            for (std::size_t index = 0; index < function->parameters.size(); index++)
            {
                String id = String::from(index) + ": ";
                bool is_last = index == function->parameters.size() - 1;
                tree_of_impl(function->parameters[index], result, new_prefix, is_last, id);
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            auto member = static_cast<Member*>(type);
            result += branch + String("member ptr").coloring(BUILTIN_COLOR) + "\n";
            tree_of_impl(member->class_type, result, new_prefix, false, "C: ");
            tree_of_impl(member->member_type, result, new_prefix, true, "M: ");
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            auto template_ = static_cast<Template*>(type);
            auto name = String(template_->name).coloring(TYPE_COLOR);
            auto modifier = (String("[TEMPLATE] ") + template_->modifier).coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
            for (std::size_t index = 0; index < template_->parameters.size(); index++)
            {
                String id = String::from(index) + ": ";
                bool is_last = index == template_->parameters.size() - 1;
                tree_of_impl(template_->parameters[index], result, new_prefix, is_last, id);
            }
        }
        else if (type->Kind() == TypeKind::NTTP)
        {
            auto nttp = static_cast<NTTP*>(type);
            auto name = String(nttp->name).coloring(NTTP_COLOR);
            auto modifier = String("[NTTP] ").coloring(MODIFIER_COLOR);
            result += format("{}{} {}\n", branch, name, modifier);
        }
    }

    constexpr void name_of_impl(Type* type, String& result)
    {

        if (type->Kind() == TypeKind::BASIC)
        {
            auto basic = static_cast<BasicType*>(type);
            if (basic->modifier.find("]") != std::string_view::npos)
            {
                result += basic->modifier.substr(1, basic->modifier.size() - 2);
                result += " ";
            }
            result += basic->name;
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            auto pointer = static_cast<Pointer*>(type);
            name_of_impl(pointer->pointee, result);
            result += String("*");
            if (pointer->modifier.find("]") != std::string_view::npos)
            {
                result += " ";
                result += pointer->modifier.substr(1, pointer->modifier.size() - 2);
            }
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            auto ref = static_cast<Reference*>(type);
            name_of_impl(ref->pointee, result);
            if (ref->modifier.find("]") != std::string_view::npos)
            {
                result += ref->modifier.substr(1, ref->modifier.size() - 2);
            }
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            auto array = static_cast<Array*>(type);
            name_of_impl(array->element, result);
            result += String("[") + String::from(array->size) + String("]");
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            auto function = static_cast<Function*>(type);
            name_of_impl(function->return_type, result);
            result += String("(");
            for (auto& parameter : function->parameters)
            {
                name_of_impl(parameter, result);
                if (&parameter != &function->parameters.back())
                {
                    result += ", ";
                }
            }
            if (function->modifier.find("...") != std::string_view::npos)
            {
                result += ", ...";
            }
            result += ")";
            auto last = function->modifier.rfind("]");
            if (last != std::string_view::npos)
            {
                auto start = function->modifier.rfind("[") + 1;
                result += " ";
                result += function->modifier.substr(start, last - start);
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            auto member = static_cast<Member*>(type);
            name_of_impl(member->class_type, result);
            result += String("::* ");
            name_of_impl(member->member_type, result);
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            auto template_ = static_cast<Template*>(type);
            result += template_->name;
            result += "<";
            for (auto& parameter : template_->parameters)
            {
                name_of_impl(parameter, result);
                if (&parameter != &template_->parameters.back())
                {
                    result += ", ";
                }
            }
            result += ">";
        }
        else if (type->Kind() == TypeKind::NTTP)
        {
            auto nttp = static_cast<NTTP*>(type);
            result += nttp->name;
        }
    }

    template <typename T>
    constexpr auto name_to_array()
    {
        constexpr auto length = []
        {
            magic::String result{};
            Type* type = magic::details::parse<T>(false);
            magic::details::name_of_impl(type, result);
            delete type;
            return result.size();
        }();
        std::array<char, length> buffer = {};
        magic::String str;
        Type* type = magic::details::parse<T>(false);
        magic::details::name_of_impl(type, str);
        delete type;
        std::copy(str.begin(), str.end(), buffer.begin());
        return buffer;
    }

    template <typename T>
    struct type_name_storage
    {
        constexpr static auto storage = name_to_array<T>();
        constexpr static auto name = std::string_view(storage.data(), storage.size());
    };

} // namespace magic::details

namespace magic
{
    template <typename T>
    constexpr auto tree_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(false);
        magic::details::tree_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr auto full_tree_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(true);
        magic::details::tree_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr std::string_view full_name_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(true);
        magic::details::name_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr std::string_view name_of()
    {
        return magic::details::type_name_storage<T>::name;
    }

    using magic::details::raw_name_of;

    template <>
    struct type_info<std::size_t>
    {
        constexpr static auto name = "std::size_t";
    };

    template <>
    struct type_info<std::string>
    {
        constexpr static auto name = "std::string";
    };

    template <>
    struct type_info<std::string_view>
    {
        constexpr static auto name = "std::string_view";
    };

    template <typename T>
    struct type_info<std::vector<T>>
    {
        constexpr static auto storage = []()
        {
            constexpr auto name = name_of<T>();
            constexpr auto length = name.length() + 13;
            std::array<char, length> buffer = {};
            constexpr auto temp = "std::vector<";
            for (std::size_t i = 0; i < 12; i++) { buffer[i] = temp[i]; }
            std::copy(name.begin(), name.end(), buffer.begin() + 12);
            buffer[length - 1] = '>';
            return buffer;
        }();

        constexpr static auto name = std::string_view(storage.data(), storage.size());
    };

} // namespace magic

#endif // MAGIC_CPP_TYPE_H