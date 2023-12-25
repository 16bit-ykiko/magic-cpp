#ifndef MAGIC_CPP_MACRO_H
#define MAGIC_CPP_MACRO_H
// clang-format off

    #if __cplusplus >= 202002L && defined(__has_include) && __has_include(<source_location>)
        #include <source_location>
    #endif

    // support for source_location
    
    // fix for msvc-clang, whose function_name will not show the full name of the function
    #if __clang__ && _MSC_VER
        #define MAGIC_CPP_FUNCTION_NAME (__FUNCSIG__)
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #elif __cpp_lib_source_location
        #define MAGIC_CPP_FUNCTION_NAME (std::source_location::current().function_name())
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #elif (__clang__ || __GNUC__) && (!_MSC_VER)
        #define MAGIC_CPP_FUNCTION_NAME (__PRETTY_FUNCTION__)
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #elif _MSC_VER
        #define MAGIC_CPP_FUNCTION_NAME (__FUNCSIG__)
        #define MAGIC_CPP_COMPILER_SUPPORT 1

    #else
        #define MAGIC_CPP_COMPILER_SUPPORT 0

    #endif // source_location

// clang-format on
#endif // MAGIC_CPP_MACRO_H