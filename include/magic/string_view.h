#ifndef MAGIC_CPP_STRING_VIEW_H
#define MAGIC_CPP_STRING_VIEW_H

#include <cstddef>
#include <cstdint>

namespace magic
{
    /*
     * string_view used for compile time
     * that does not have array bounds checking
     * for faster compile speed
     */

    class string_view
    {
        const char* m_Data;
        std::size_t m_Size;

      public:
        constexpr string_view() noexcept
        {
            m_Data = nullptr;
            m_Size = 0;
        }

        constexpr string_view(const string_view& other) noexcept = default;

        constexpr string_view(std::nullptr_t) = delete;

        constexpr string_view& operator=(const string_view& view) noexcept = default;

        constexpr friend bool operator==(const string_view& lhs, const string_view& rhs) noexcept
        {
            if (lhs.size() != rhs.size())
            {
                return false;
            }
            for (std::size_t i = 0; i < lhs.size(); i++)
            {
                if (lhs[i] != rhs[i])
                {
                    return false;
                }
            }
            return true;
        }

        constexpr friend bool operator!=(const string_view& lhs, const string_view& rhs) noexcept { return !(lhs == rhs); }

        constexpr string_view(const char* str) : m_Data(str), m_Size(0)
        {
            while (str[m_Size] != '\0') { m_Size++; }
        }

        constexpr string_view(const char* str, std::size_t N) : m_Data(str), m_Size(N) {}

      public:
        constexpr inline static std::size_t npos = UINTMAX_MAX;

        [[nodiscard]] constexpr bool empty() const noexcept { return m_Size == 0; }

        [[nodiscard]] constexpr std::size_t size() const noexcept { return m_Size; }

        [[nodiscard]] constexpr std::size_t length() const noexcept { return m_Size; }

        [[nodiscard]] constexpr const char* begin() const noexcept { return m_Data; }

        [[nodiscard]] constexpr const char* end() const noexcept { return m_Data + m_Size; }

        [[nodiscard]] constexpr const char* data() const noexcept { return m_Data; }

        [[nodiscard]] constexpr char operator[](std::size_t index) const noexcept { return m_Data[index]; }

        [[nodiscard]] constexpr std::size_t find(string_view str) const noexcept
        {
            for (std::size_t i = 0; i < m_Size; i++)
            {
                if (m_Data[i] == str[0])
                {
                    bool found = true;
                    for (std::size_t j = 1; j < str.size(); j++)
                    {
                        if (m_Data[i + j] != str[j])
                        {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                    {
                        return i + str.size();
                    }
                }
            }
            return npos;
        }

        [[nodiscard]] constexpr std::size_t rfind(string_view str) const noexcept
        {
            for (std::size_t i = m_Size - 1; i > 0; i--)
            {
                if (m_Data[i] == str[0])
                {
                    bool found = true;
                    for (std::size_t j = 1; j < str.size(); j++)
                    {
                        if (m_Data[i + j] != str[j])
                        {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                    {
                        return i;
                    }
                }
            }
            return npos;
        }

        [[nodiscard]] constexpr string_view substr(std::size_t start, std::size_t end) const noexcept
        {
            return {m_Data + start, end - start};
        }
    };
} // namespace magic
#endif