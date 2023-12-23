#ifndef MAGIC_CPP_STRING_H
#define MAGIC_CPP_STRING_H

#define NOCOLOR 0
#define MODIFIER_COLOR 0x98C379
#define TYPE_COLOR 0xE5C07B
#define BUILTIN_COLOR 0xC678DD
#define NTTP_COLOR 0xD19A66
#define TAG_COLOR 0x5C6370

#include "string_view.h"
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

namespace magic
{
    class String : public std::vector<char>
    {

      public:
        constexpr String() = default;

        constexpr String(const String& other) = default;

        constexpr String(String&& other) = default;

        constexpr String& operator=(String&& other) = default;

        constexpr ~String() = default;

        constexpr String(const char* str) : String(string_view(str)) {}

        constexpr String(const char* str, std::size_t N) : String(string_view(str, N)) {}

        constexpr String(string_view str)
        {
            for (std::size_t i = 0; i < str.size(); i++) { push_back(str[i]); }
        }

        constexpr String& operator+=(const String& other)
        {

            for (std::size_t i = 0; i < other.size(); i++) { push_back(other[i]); }
            return *this;
        }

        constexpr operator string_view() const { return string_view(data(), size()); }

        friend auto& operator<<(std::ostream& os, const String& str)
        {
            os << std::string_view(str.data(), str.size());
            return os;
        }
    };

    template <std::size_t N>
    struct static_string : std::array<char, N + 1>
    {
        constexpr static_string() = default;

        constexpr static_string(const char (&str)[N + 1])
        {
            for (std::size_t i = 0; i < N; i++) { this->data()[i] = str[i]; }
            this->data()[N] = '\0';
        }

        constexpr std::size_t length() const { return N; }
    };

    template <std::size_t N>
    static_string(const char (&str)[N]) -> static_string<N - 1>;

    template <static_string fmt>
    constexpr auto parse_fmt()
    {
        constexpr std::size_t length = []
        {
            std::size_t count = 0;
            for (std::size_t i = 0; i < fmt.length(); i++)
            {
                if (i + 1 < fmt.length() && fmt[i] == '{' && fmt[i + 1] == '}')
                {
                    count++;
                    i++;
                }
            }
            return count;
        }();
        std::array<std::size_t, length> result{};
        std::size_t index = 0;
        for (std::size_t i = 0; i < fmt.length(); i++)
        {
            if (i + 1 < fmt.length() && fmt[i] == '{' && fmt[i + 1] == '}')
            {
                result[index++] = i;
                i++;
            }
        }
        return result;
    }

    constexpr auto integer_to_buffer(char* buffer, std::size_t N) -> std::size_t
    {
        if (N == 0)
        {
            buffer[0] = '0';
            return 1;
        }
        std::size_t digitCount = 0;
        for (std::size_t temp = N; temp != 0; digitCount++, temp /= 10) {}
        auto temp = digitCount;
        while (N != 0)
        {
            buffer[digitCount - 1] = '0' + (N % 10);
            digitCount--;
            N /= 10;
        }
        return temp;
    }

    template <static_string fmt>
    constexpr auto format(auto... args)
    {
        constexpr auto indices = parse_fmt<fmt>();
        if constexpr (indices.size() != sizeof...(args))
        {
            static_assert(indices.size() == sizeof...(args), "format string and arguments count mismatch");
        }
        else
        {
            auto tuple = std::forward_as_tuple(args...);
            std::array<char, 500> buffer{};
            std::size_t index = 0;
            std::size_t buffer_index = 0;
            [&]<std::size_t... Is>(std::index_sequence<Is...>)
            {
                auto foreach = [&]<std::size_t I>()
                {
                    std::copy(fmt.data() + index, fmt.data() + indices[I], buffer.data() + buffer_index);
                    buffer_index += indices[I] - index;
                    index = indices[I] + 2;
                    using T = std::decay_t<std::tuple_element_t<I, decltype(tuple)>>;
                    if constexpr (std::is_integral_v<T>)
                    {
                        auto N = std::get<I>(tuple);
                        auto start = buffer.data() + buffer_index;
                        buffer_index += integer_to_buffer(start, N);
                    }
                    else if constexpr (std::is_convertible_v<T, string_view>)
                    {
                        auto str = string_view{std::get<I>(tuple)};
                        std::copy(str.data(), str.data() + str.size(), buffer.data() + buffer_index);
                        buffer_index += str.size();
                    }
                };
                (foreach.template operator()<Is>(), ...);
            }(std::make_index_sequence<indices.size()>{});
            std::copy(fmt.data() + index, fmt.data() + fmt.length(), buffer.data() + buffer_index);
            buffer_index += fmt.length() - index;
            return String{buffer.data(), buffer_index};
        }
    }

    constexpr auto colorize(string_view text, uint32_t color) noexcept
    {
        if constexpr (!NOCOLOR)
        {
            if (text.size() == 0)
            {
                return String{};
            }
            uint8_t R = (color >> 16) & 0xFF;
            uint8_t G = (color >> 8) & 0xFF;
            uint8_t B = color & 0xFF;
            return format<"\033[38;2;{};{};{}m{}\033[0m">(R, G, B, text);
        }
        else
        {
            return text;
        }
    }
} // namespace magic

#endif // MAGIC_CPP_STRING_H