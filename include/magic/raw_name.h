#ifndef MAGIC_CPP_RAW_NAME_H
#define MAGIC_CPP_RAW_NAME_H
#include <string_view>

#if __clang__ || __GNUC__
#define METAINFO                                                                                                                           \
    std::string_view name = __PRETTY_FUNCTION__;                                                                                           \
    std::size_t first = name.find("T =") + 4;                                                                                              \
    std::size_t last = name.rfind("]");                                                                                                    \
    return name.substr(first, last - first);

#elif _MSC_VER
#define METAINFO                                                                                                                           \
    std::string_view name = __FUNCSIG__;                                                                                                   \
    std::string_view prefix = "name_of<class ";                                                                                            \
    std::size_t last = name.rfind(">(");                                                                                                   \
    std::size_t first = name.find(prefix);                                                                                                 \
    if (first == std::string_view::npos)                                                                                                   \
    {                                                                                                                                      \
        prefix = "name_of<struct ";                                                                                                        \
        first = name.find(prefix);                                                                                                         \
    }                                                                                                                                      \
    if (first == std::string_view::npos)                                                                                                   \
    {                                                                                                                                      \
        prefix = "name_of<";                                                                                                               \
        first = name.find(prefix);                                                                                                         \
    }                                                                                                                                      \
    first += prefix.size();                                                                                                                \
    return name.substr(first, last - first);

#else
static_assert(false, "Unsupported compiler");
#endif

namespace magic::details
{
    template <typename T>
    constexpr auto raw_name_of(){METAINFO};

    template <auto T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename...> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto...> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, typename, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, typename, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, auto, typename, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, typename, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, auto, auto, typename> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, typename, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, auto, typename, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, typename, auto, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <auto, typename, auto, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

    template <template <typename, auto, auto, auto> class T>
    constexpr auto raw_name_of(){METAINFO};

} // namespace magic::details
#undef METAINFO

#endif // MAGIC_CPP_RAW_NAME_H