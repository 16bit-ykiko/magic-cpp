#ifndef MAGIC_CPP_VISUALIZE_H
#define MAGIC_CPP_VISUALIZE_H

#if __has_include(<format>)
#include <format>
#endif

#include "customized_type_name.h"
#include "type_tree.h"
#include <array>

namespace magic
{
    struct HighlightConfig
    {
        std::uint32_t type;
        std::uint32_t nttp;
        std::uint32_t tmpl;
        std::uint32_t builtin;
        std::uint32_t modifier;
        std::uint32_t tag;
    };

    constexpr static inline HighlightConfig Dark = {
        .type = 0xE5C07B,
        .nttp = 0xD19A66,
        .tmpl = 0x0087CE,
        .builtin = 0xC678DD,
        .modifier = 0x98C379,
        .tag = 0x5C6370,
    };

    constexpr static inline HighlightConfig Light = {
        .type = 0x4285F4,     // Light blue for type
        .nttp = 0x0F9D58,     // Green for non-type template parameter
        .tmpl = 0xFF5722,     // Orange for template
        .builtin = 0x7C4DFF,  // Purple for built-in
        .modifier = 0xFBC02D, // Yellow for modifier
        .tag = 0x03A9F4,      // Light blue for tag
    };

} // namespace magic

namespace magic::details
{

#ifdef __cpp_lib_format
    using std::format;
#else
    /**
     * the format function below is just used for internal. No fmt args checking.
     */
    template <typename... Args>
    std::string format(std::string_view fmt, Args&&... args)
    {
        std::array<char, 256> buffer{};
        std::size_t args_index = 0;
        std::array<std::string, sizeof...(Args)> args_str;
        auto make_str = [&](auto&& arg)
        {
            using Type = std::decay_t<decltype(arg)>;
            if constexpr (std::is_integral_v<Type> || std::is_floating_point_v<Type>)
            {
                args_str[args_index] = std::to_string(arg);
            }
            else if constexpr (std::is_same_v<Type, std::string>)
            {
                args_str[args_index] = std::forward<decltype(arg)>(arg);
            }
            else if constexpr (std::is_convertible_v<Type, std::string>)
            {
                args_str[args_index] = std::forward<decltype(arg)>(arg);
            }
            args_index += 1;
        };
        (make_str(std::forward<decltype(args)>(args)), ...);

        args_index = 0;
        std::size_t buffer_index = 0;
        for (std::size_t fmt_index = 0; fmt_index < fmt.size();)
        {
            if (fmt[fmt_index] == '{')
            {
                fmt_index += 2;
                std::size_t size = args_str[args_index].size();
                memcpy(buffer.data() + buffer_index, args_str[args_index].data(), size);
                buffer_index += size;
                args_index += 1;
            }
            else
            {
                buffer[buffer_index] = fmt[fmt_index];
                buffer_index += 1;
                fmt_index += 1;
            }
        }
        return std::string{buffer.data(), buffer_index};
    }
#endif // __cpp_lib_format

    template <typename T>
    std::string colorize(T&& text, std::uint32_t color)
    {
        uint8_t R = (color >> 16) & 0xFF;
        uint8_t G = (color >> 8) & 0xFF;
        uint8_t B = color & 0xFF;
        return format("\033[38;2;{};{};{}m{}\033[0m", R, G, B, std::forward<T>(text));
    }

    inline void visualize_impl(std::string& result, Type* type, std::string prefix = "", bool is_last = true, std::string tag = "",
                               const HighlightConfig& color = Dark)
    {
        std::string branch = format("{}{}{}", prefix, is_last ? "└── " : "├── ", colorize(tag, color.tag));
        prefix += (is_last ? "    " : "│   ");

        for (std::size_t i = 0; i < tag.size(); i++) { prefix.push_back(' '); }
        if (type->Kind() == TypeKind::BASIC)
        {
            BasicType* basic = static_cast<BasicType*>(type);
            std::string name = colorize(std::move(basic->name), color.type);
            std::string modifier;
            modifier += basic->is_const ? "const" : "";
            modifier += basic->is_volatile ? (modifier.empty() ? "volatile" : " volatile") : "";
            if (modifier.empty())
            {
                result += format("{}{}\n", branch, std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), color.modifier);
                result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            }
        }
        else if (type->Kind() == TypeKind::POINTER)
        {
            Pointer* pointer = static_cast<Pointer*>(type);
            std::string name = colorize("ptr", color.builtin);
            std::string modifier;
            modifier += pointer->is_const ? "const" : "";
            modifier += pointer->is_volatile ? (modifier.empty() ? "volatile" : " volatile") : "";
            if (modifier.empty())
            {
                result += format("{}{}\n", branch, std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), color.modifier);
                result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            }
            visualize_impl(result, pointer->pointee, prefix, true, "", color);
        }
        else if (type->Kind() == TypeKind::REFERENCE)
        {
            Reference* ref = static_cast<Reference*>(type);
            std::string name = colorize("ref", color.builtin);
            std::string modifier = colorize(ref->is_lvalue_ref ? "&" : "&&", color.modifier);
            if (modifier.empty())
            {
                result += format("{}{}\n", branch, std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), color.modifier);
                result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            }
            visualize_impl(result, ref->referencee, prefix, true, "", color);
        }
        else if (type->Kind() == TypeKind::ARRAY)
        {
            Array* array = static_cast<Array*>(type);
            std::string name = colorize("array", color.builtin);
            std::string modifier = colorize(format("N = {}", array->size), color.modifier);
            result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            visualize_impl(result, array->element, prefix, true, "", color);
        }
        else if (type->Kind() == TypeKind::FUNCTION)
        {
            Function* function = static_cast<Function*>(type);
            std::string name = colorize("function", color.builtin);
            std::string modifier;
            modifier += function->isConst() ? "const" : "";
            modifier += function->isVolatile() ? (modifier.empty() ? "volatile" : " volatile") : "";
            modifier += function->isLvalueRef() ? "&" : "";
            modifier += function->isRvalueRef() ? "&&" : "";
            modifier += function->isNoexcept() ? (modifier.empty() ? "noexcept" : " noexcept") : "";
            if (modifier.empty())
            {
                result += format("{}{}\n", branch, std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), color.modifier);
                result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            }

            std::vector<Type*>& params = function->parameters;
            const auto size = params.size();

            visualize_impl(result, function->return_type, prefix, false, "R: ", color);

            for (std::size_t index = 0; index < size; index++)
            {
                Type* param = params[index];
                is_last = (index == size - 1) && !function->isVariadic();
                visualize_impl(result, param, prefix, is_last, format("{}: ", index), color);
            }

            tag = colorize(format("{}: ", size), color.tag);

            if (function->isVariadic())
            {
                result += format("{}{}{}\n", prefix + "└── ", tag, colorize("<...>", color.builtin));
            }
            else if (params.empty())
            {
                result += format("{}{}{}\n", prefix + "└── ", tag, colorize("<void>", color.builtin));
            }
        }
        else if (type->Kind() == TypeKind::MEMBER)
        {
            Member* member = static_cast<Member*>(type);
            std::string name = colorize("member ptr", color.builtin);
            std::string modifier;
            modifier += member->is_const ? "const" : "";
            modifier += member->is_volatile ? (modifier.empty() ? "volatile" : " volatile") : "";
            if (modifier.empty())
            {
                result += format("{}{}\n", branch, std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), color.modifier);
                result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));
            }

            visualize_impl(result, member->class_type, prefix, false, "C: ", color);
            visualize_impl(result, member->member_type, prefix, true, "M: ", color);
        }
        else if (type->Kind() == TypeKind::TEMPLATE)
        {
            Template* template_ = static_cast<Template*>(type);
            std::string name = colorize(std::move(template_->name), color.tmpl);
            std::string modifier = "template";
            modifier += template_->is_const ? " const" : "";
            modifier += template_->is_volatile ? " volatile" : "";
            modifier = colorize(std::move(modifier), color.modifier);
            result += format("{}{} [{}]\n", branch, std::move(name), std::move(modifier));

            for (std::size_t index = 0; index < template_->parameters.size(); index++)
            {
                Type* parameter = template_->parameters[index];
                is_last = index == template_->parameters.size() - 1;
                visualize_impl(result, parameter, prefix, is_last, format("{}: ", index), color);
            }
        }
        else if (type->Kind() == TypeKind::NTTP)
        {
            NTTP* nttp = static_cast<NTTP*>(type);
            std::string name = colorize("nttp", color.builtin);
            result += format("{}{}\n", branch, std::move(name));
            visualize_impl(result, nttp->type, prefix, false, "T: ", color);
            name = colorize(nttp->name, color.nttp);
            tag = colorize("V: ", color.tag);
            result += format("{}{}{}\n", prefix + "└── ", tag, std::move(name));
        }
    }

    inline void display_name_of(std::string& result, Type* type, bool is_full_name) {}

} // namespace magic::details

namespace magic
{
    template <typename T>
    std::string visualize(const HighlightConfig& color = Dark, bool is_full_name = false)
    {
        std::string result;
        auto* type = details::parse<T>(is_full_name);
        details::visualize_impl(result, type, "", true, "", color);
        delete type;
        return result;
    }

} // namespace magic

#endif