#ifndef MAGIC_CPP_TYPE_H
#define MAGIC_CPP_TYPE_H
#include "parse.h"

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
    };
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
            result += branch + coloring(basic->name, TYPE_COLOR) + " " + coloring(basic->modifier, MODIFIER_COLOR) + "\n";
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            auto pointer = static_cast<Pointer*>(type);
            result += branch + coloring("ptr", BUILTIN_COLOR) + " " + coloring(pointer->modifier, MODIFIER_COLOR) + "\n";
            tree_of_impl(pointer->pointee, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            auto ref = static_cast<Reference*>(type);
            result += branch + coloring("ref", BUILTIN_COLOR) + " " + coloring(ref->modifier, MODIFIER_COLOR) + "\n";
            tree_of_impl(ref->pointee, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            auto array = static_cast<Array*>(type);
            result += branch + coloring("array", BUILTIN_COLOR) + " " +
                      coloring("[N = " + String::from(array->size) + "]", MODIFIER_COLOR) + "\n";
            tree_of_impl(array->element, result, new_prefix, true);
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            auto function = static_cast<Function*>(type);
            result += branch + coloring("function", BUILTIN_COLOR) + " " + coloring(function->modifier, MODIFIER_COLOR) + "\n";
            tree_of_impl(function->return_type, result, new_prefix, false, "R: ");
            std::size_t index = 0;
            for (auto parameter : function->parameters)
            {
                index += 1;
                String id = String::from(index - 1) + ": ";
                tree_of_impl(parameter, result, new_prefix, index == function->parameters.size(), id);
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            auto member = static_cast<Member*>(type);
            result += branch + coloring("member ptr", BUILTIN_COLOR) + "\n";
            tree_of_impl(member->class_type, result, new_prefix, false, "C: ");
            tree_of_impl(member->member_type, result, new_prefix, true, "M: ");
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            auto template_ = static_cast<Template*>(type);
            result += branch + coloring(template_->name, TYPE_COLOR) + " " + coloring("[TEMPLATE]", MODIFIER_COLOR) + " " +
                      coloring(template_->modifier, MODIFIER_COLOR) + "\n";
            std::size_t index = 0;
            for (auto parameter : template_->parameters)
            {
                index += 1;
                String id = String::from(index - 1) + ": ";
                tree_of_impl(parameter, result, new_prefix, index == template_->parameters.size(), id);
            }
        }
        else if (type->Kind() == TypeKind::NTTP)
        {
            auto nttp = static_cast<NTTP*>(type);
            result += branch + coloring(nttp->name, NTTP_COLOR) + " " + coloring("[NTTP]", MODIFIER_COLOR) + "\n";
        }
    }

    constexpr void name_of_impl(Type* type, String& result)
    {
        if (type->Kind() == TypeKind::BASIC)
        {
            auto basic = static_cast<BasicType*>(type);
            result += basic->modifier.substr(1, basic->modifier.size() - 2) + String(" ") + basic->name;
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            auto pointer = static_cast<Pointer*>(type);
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            auto ref = static_cast<Reference*>(type);
            name_of_impl(ref->pointee, result);
            result += " ";
            result += ref->modifier.substr(1, ref->modifier.size() - 2);
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
            result += function->modifier.substr(1, function->modifier.size() - 2);
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            auto member = static_cast<Member*>(type);
            if (member->member_type->Kind() == TypeKind::FUNCTION)
            {
                auto function = static_cast<Function*>(member->member_type);
                result += function->modifier.substr(1, function->modifier.size() - 2);
                name_of_impl(function->return_type, result);
                result += " (";
                name_of_impl(member->class_type, result);
                result += "::*)(";
                for (auto& parameter : function->parameters)
                {
                    name_of_impl(parameter, result);
                    if (&parameter != &function->parameters.back())
                    {
                        result += ", ";
                    }
                }
                result += ")";
            }
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
} // namespace magic::details

namespace magic
{
    template <typename T>
    constexpr auto tree_of()
    {
        magic::String result;
        magic::details::tree_of_impl(magic::details::parse<T>(false), result);
        return result;
    }

    template <typename T>
    constexpr auto full_tree_of()
    {
        magic::String result;
        magic::details::tree_of_impl(magic::details::parse<T>(true), result);
        return result;
    }

    using magic::details::raw_name_of;
} // namespace magic

#endif // MAGIC_CPP_TYPE_H