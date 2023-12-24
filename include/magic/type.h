#ifndef MAGIC_CPP_TYPE_H
#define MAGIC_CPP_TYPE_H

#include "parse.h"
#include "string.h"

namespace magic::details
{
    /**
     * used to tree_of_impl the type tree and output the final string
     */

    constexpr void tree_of_impl(Type* type, String& result, String prefix = "", bool is_last = true, String tag = "")
    {
        String branch = format<"{}{}{}">(prefix, is_last ? "└── " : "├── ", colorize(tag, TAG_COLOR));
        prefix += (is_last ? "    " : "│   ");
        for (std::size_t i = 0; i < tag.size(); i++) { prefix.push_back(' '); }
        if (type->Kind() == TypeKind::BASIC)
        {
            BasicType* basic = static_cast<BasicType*>(type);
            String name = colorize(basic->name, TYPE_COLOR);
            String modifier = colorize(basic->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            Pointer* pointer = static_cast<Pointer*>(type);
            String name = colorize("ptr", BUILTIN_COLOR);
            String modifier = colorize(pointer->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            tree_of_impl(pointer->pointee, result, prefix, true);
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            Reference* ref = static_cast<Reference*>(type);
            String name = colorize("ref", BUILTIN_COLOR);
            String modifier = colorize(ref->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            tree_of_impl(ref->pointee, result, prefix, true);
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            Array* array = static_cast<Array*>(type);
            String name = colorize("array", BUILTIN_COLOR);
            String modifier = colorize(format<"[N = {}]">(array->size), MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            tree_of_impl(array->element, result, prefix, true);
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            Function* function = static_cast<Function*>(type);
            String name = colorize("function", BUILTIN_COLOR);
            String modifier = colorize(function->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            tree_of_impl(function->return_type, result, prefix, function->parameters.size() == 0, "R: ");
            for (std::size_t index = 0; index < function->parameters.size(); index++)
            {
                Type* parameter = function->parameters[index];
                bool is_last = index == function->parameters.size() - 1;
                tree_of_impl(parameter, result, prefix, is_last, format<"{}: ">(index));
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            Member* member = static_cast<Member*>(type);
            String name = colorize("member", BUILTIN_COLOR);
            String modifier = colorize(member->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            tree_of_impl(member->class_type, result, prefix, false, "C: ");
            tree_of_impl(member->member_type, result, prefix, true, "M: ");
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            Template* template_ = static_cast<Template*>(type);
            String name = colorize(template_->name, TYPE_COLOR);
            String modifier = colorize(String("[TEMPLATE] ") += template_->modifier, MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
            for (std::size_t index = 0; index < template_->parameters.size(); index++)
            {
                Type* parameter = template_->parameters[index];
                bool is_last = index == template_->parameters.size() - 1;
                tree_of_impl(parameter, result, prefix, is_last, format<"{}: ">(index));
            }
        }
        else if (type->Kind() == TypeKind::NTTP)
        {
            NTTP* nttp = static_cast<NTTP*>(type);
            String name = colorize(nttp->name, NTTP_COLOR);
            String modifier = colorize("[NTTP]", MODIFIER_COLOR);
            result += format<"{}{} {}\n">(branch, std::move(name), std::move(modifier));
        }
    }

    constexpr void name_of_impl(Type* type, String& result)
    {

        if (type->Kind() == TypeKind::BASIC)
        {
            BasicType* basic = static_cast<BasicType*>(type);
            if (!basic->modifier.empty())
            {
                result += basic->modifier.substr(1, basic->modifier.size() - 1);
                result += " ";
            }
            result += basic->name;
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            Pointer* pointer = static_cast<Pointer*>(type);
            name_of_impl(pointer->pointee, result);
            result += String("*");
            if (!pointer->modifier.empty())
            {
                result += " ";
                result += pointer->modifier.substr(1, pointer->modifier.size() - 1);
            }
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            Reference* ref = static_cast<Reference*>(type);
            name_of_impl(ref->pointee, result);
            if (!ref->modifier.empty())
            {
                result += ref->modifier.substr(1, ref->modifier.size() - 1);
            }
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            Array* array = static_cast<Array*>(type);
            name_of_impl(array->element, result);
            result += format<"[{}]">(array->size);
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            Function* function = static_cast<Function*>(type);
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
            if (function->modifier.find("...") != string_view::npos)
            {
                result += ", ...";
            }
            result += ")";
            if (function->modifier.size() > 5)
            {
                result += " ";
                result += function->modifier.substr(5, function->modifier.size() - 1);
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            Member* member = static_cast<Member*>(type);
            name_of_impl(member->class_type, result);
            result += String("::* ");
            name_of_impl(member->member_type, result);
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            Template* template_ = static_cast<Template*>(type);
            result += template_->name;
            result += "<";
            auto parameters = template_->parameters;
            for (auto& parameter : parameters)
            {
                name_of_impl(parameter, result);
                if (&parameter != &parameters.back())
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
        constexpr static auto name = string_view(storage.data(), storage.size());
    };

} // namespace magic::details

namespace magic
{
    template <typename T>
    constexpr String tree_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(false);
        magic::details::tree_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr String full_tree_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(true);
        magic::details::tree_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr String full_name_of()
    {
        magic::String result;
        auto* type = magic::details::parse<T>(true);
        magic::details::name_of_impl(type, result);
        delete type;
        return result;
    }

    template <typename T>
    constexpr string_view name_of()
    {
        return magic::details::type_name_storage<T>::name;
    }

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
    struct type_info<string_view>
    {
        constexpr static auto name = "string_view";
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

        constexpr static auto name = string_view(storage.data(), storage.size());
    };

} // namespace magic

#endif // MAGIC_CPP_TYPE_H