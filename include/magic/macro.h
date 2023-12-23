#ifndef MAGIC_CPP_MACRO_H
#define MAGIC_CPP_MACRO_H
// clang-format off

    // support for source_location
    #if __cplusplus >= 202002L && defined(__has_include) && __has_include(<source_location>)
        #include <source_location>
        #define MAGIC_CPP_FUNCTION_NAME (std::source_location::current().function_name())
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #elif (__clang__ || __GNUC__)
        #define MAGIC_CPP_FUNCTION_NAME (__PRETTY_FUNCTION__)
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #elif _MSC_VER
        #define MAGIC_CPP_FUNCTION_NAME (__FUNCSIG__)
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #else
        #define MAGIC_CPP_COMPILER_SUPPORT 0

    #endif // source_location

    // support for constexpr virtual functions
    #if __cplusplus >= 202002L
        #define MAGIC_CPP_CONSTEXPR constexpr
    #else
        #define MAGIC_CPP_CONSTEXPR
    #endif // constexpr_virtual

    #if __cplusplus >= 202002L
        #define MAGIC_CPP_CONSTEXPR_VIRTUAL constexpr virtual
    #else
        #define MAGIC_CPP_CONSTEXPR_VIRTUAL virtual
    #endif // constexpr_virtual

// clang-format on
#endif // MAGIC_CPP_MACRO_H