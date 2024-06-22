#ifndef MAGIC_CPP_VISUALIZE_H
#define MAGIC_CPP_VISUALIZE_H

#include <cstdio>
#include <iterator>
#include <numeric> // for std::accumulate
#include <utility> // for std::pair
#include <vector>  // for std::vector
#if __has_include(<format>)
#include <format>
#endif

#include "customization.h"
#include "type_tree.h"
#include <array>
#include <cstring>

#include <cstdint> // for std::uint*_t

namespace magic
{
    struct VisualizeOption
    {
        bool utf_support = true;
        bool color_support = true;
        bool full_name = false;
    };

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
        .type = 0x4285F4,
        .nttp = 0x0F9D58,
        .tmpl = 0xFF5722,
        .builtin = 0x7C4DFF,
        .modifier = 0xFBC02D,
        .tag = 0x03A9F4,
    };

    template <typename T>
    std::string visualize(const VisualizeOption& option = {}, const HighlightConfig& color = Dark);

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
        std::array<std::string, sizeof...(Args)> args_str{};
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
            else if constexpr (std::is_constructible_v<std::string, Type>)
            {
                args_str[args_index] = std::string{arg};
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

    class VisualizationImpl
    {
        template <typename T>
        inline friend std::string magic::visualize(const VisualizeOption& option, const HighlightConfig& color);
        struct DirectoryTreeSymbols
        {
            std::string_view branch;
            std::string_view vertical;
            std::string_view corner;
            std::string_view space;
        };

        std::string result;
        HighlightConfig config;
        DirectoryTreeSymbols symbols;
        bool color_support;

      public:
        VisualizationImpl(const VisualizeOption& option, const HighlightConfig& config)
            : result(), config(config), color_support(option.color_support)
        {
            if (option.utf_support)
            {
                symbols = {"├── ", "│   ", "└── ", "    "};
            }
            else
            {
                symbols = {"|-- ", "|   ", "`-- ", "    "};
            }
        }

        template <typename T>
        std::string colorize(T&& text, std::uint32_t color)
        {
            if (!color_support)
            {
                return std::forward<T>(text);
            }
            std::uint8_t R = (color >> 16) & 0xFF;
            std::uint8_t G = (color >> 8) & 0xFF;
            std::uint8_t B = color & 0xFF;
            return format("\033[38;2;{};{};{}m{}\033[0m", R, G, B, std::forward<T>(text));
        }

        void visualize(std::string&& branch, std::string&& name, std::string&& modifier)
        {
            if (modifier.empty())
            {
                result += format("{}{}\n", std::move(branch), std::move(name));
            }
            else
            {
                modifier = colorize(std::move(modifier), config.modifier);
                result += format("{}{} [{}]\n", std::move(branch), std::move(name), std::move(modifier));
            }
        }

        void visualize(Type* type, std::string prefix, std::string tag, bool is_last)
        {
            std::string branch = format("{}{}{}", prefix, (is_last ? symbols.corner : symbols.branch), colorize(tag, config.tag));
            prefix += (is_last ? symbols.space : symbols.vertical);

            auto build_boolean_modifiers = [](const std::vector<std::pair<bool, std::string_view>>& modifier_map)
            {
                std::vector<std::string_view> modifiers;

                for (auto const& [pred, mod] : modifier_map)
                {
                    if (pred)
                    {
                        modifiers.push_back(mod);
                    }
                }

                return std::accumulate(begin(modifiers),
                                       end(modifiers),
                                       std::string{},
                                       [](const std::string& lhs, const std::string_view& rhs)
                                       { return lhs.empty() ? std::string{rhs} : lhs + " " + std::string{rhs}; });
            };
            auto build_cxx_modifiers =
                [&](bool is_const = false, bool is_volatile = false, bool is_lref = false, bool is_rref = false, bool is_noexcept = false)
            {
                return build_boolean_modifiers({
                    {is_const,    "const"   },
                    {is_volatile, "volatile"},
                    {is_lref,     "&"       },
                    {is_rref,     "&&"      },
                    {is_noexcept, "noxcept" }
                });
            };

            for (std::size_t i = 0; i < tag.size(); i++) { prefix.push_back(' '); }
            if (type->Kind() == TypeKind::BASIC)
            {
                BasicType* basic = static_cast<BasicType*>(type);
                std::string name = colorize(std::move(basic->name), config.type);
                std::string modifier = build_cxx_modifiers(basic->is_const, basic->is_volatile);
                visualize(std::move(branch), std::move(name), std::move(modifier));
            }
            else if (type->Kind() == TypeKind::POINTER)
            {
                Pointer* pointer = static_cast<Pointer*>(type);
                std::string name = colorize("ptr", config.builtin);
                std::string modifier = build_cxx_modifiers(pointer->is_const, pointer->is_volatile);
                visualize(std::move(branch), std::move(name), std::move(modifier));
                visualize(pointer->pointee, prefix, "", true);
            }
            else if (type->Kind() == TypeKind::REFERENCE)
            {
                Reference* ref = static_cast<Reference*>(type);
                std::string name = colorize("ref", config.builtin);
                std::string modifier = build_cxx_modifiers(false, false, ref->is_lvalue_ref, !ref->is_lvalue_ref);
                visualize(std::move(branch), std::move(name), std::move(modifier));
                visualize(ref->referencee, prefix, "", true);
            }
            else if (type->Kind() == TypeKind::ARRAY)
            {
                Array* array = static_cast<Array*>(type);
                std::string name = colorize("array", config.builtin);
                // non-boolean modifier
                // need not to colorize it here, `visualize(branch, name, modifier)` will perform colorize
                std::string modifier = format("N = {}", array->size);
                visualize(std::move(branch), std::move(name), std::move(modifier));
                visualize(array->element, prefix, "", true);
            }
            else if (type->Kind() == TypeKind::FUNCTION)
            {
                Function* function = static_cast<Function*>(type);
                std::string name = colorize("function", config.builtin);
                std::string modifier = build_cxx_modifiers(function->isConst(),
                                                            function->isVolatile(),
                                                            function->isLvalueRef(),
                                                            function->isLvalueRef(),
                                                            function->isNoexcept());

                visualize(std::move(branch), std::move(name), std::move(modifier));

                std::vector<Type*>& params = function->parameters;
                const auto size = params.size();
                visualize(function->return_type, prefix, "R: ", false);

                for (std::size_t index = 0; index < size; index++)
                {
                    Type* param = params[index];
                    is_last = (index == size - 1) && !function->isVariadic();
                    visualize(param, prefix, format("{}: ", index), is_last);
                }

                tag = colorize(format("{}: ", size), config.tag);
                if (function->isVariadic())
                {
                    result += format("{}{}{}\n", prefix += symbols.corner, tag, colorize("<...>", config.builtin));
                }
                else if (params.empty())
                {
                    result += format("{}{}{}\n", prefix += symbols.corner, tag, colorize("<void>", config.builtin));
                }
            }
            else if (type->Kind() == TypeKind::MEMBER)
            {
                Member* member = static_cast<Member*>(type);
                std::string name = colorize("member ptr", config.builtin);
                std::string modifier = build_cxx_modifiers(member->is_const, member->is_volatile);
                visualize(std::move(branch), std::move(name), std::move(modifier));
                visualize(member->class_type, prefix, "C: ", false);
                visualize(member->member_type, prefix, "M: ", true);
            }
            else if (type->Kind() == TypeKind::TEMPLATE)
            {
                Template* template_ = static_cast<Template*>(type);
                std::string name = colorize(std::move(template_->name), config.tmpl);
                std::string modifier = build_boolean_modifiers({
                    {true,                   "template"},
                    {template_->is_const,    "const"   },
                    {template_->is_volatile, "volatile"},
                });
                visualize(std::move(branch), std::move(name), std::move(modifier));

                for (std::size_t index = 0; index < template_->parameters.size(); index++)
                {
                    Type* parameter = template_->parameters[index];
                    is_last = index == template_->parameters.size() - 1;
                    visualize(parameter, prefix, format("{}: ", index), is_last);
                }
            }
            else if (type->Kind() == TypeKind::NTTP)
            {
                NTTP* nttp = static_cast<NTTP*>(type);
                std::string name = colorize("nttp", config.builtin);
                result += format("{}{}\n", branch, std::move(name));
                visualize(nttp->type, prefix, "T: ", false);
                name = colorize(nttp->name, config.nttp);
                tag = colorize("V: ", config.tag);
                result += format("{}{}{}\n", prefix += symbols.corner, tag, std::move(name));
            }
            else
            {
                magic::unreachable();
            }
        }
    };

    inline void display_name_of(std::string& result, Type* type, bool is_full_name) {}

} // namespace magic::details

namespace magic
{
    template <typename T>
    std::string visualize(const VisualizeOption& option, const HighlightConfig& config)
    {
        std::string result;
        Type* type = details::parse<T>(option.full_name);
        details::VisualizationImpl visualizationImpl{option, config};
        visualizationImpl.visualize(type, "", "", true);
        delete type;
        return std::move(visualizationImpl.result);
    }

} // namespace magic

#endif