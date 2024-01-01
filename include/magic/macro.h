#ifndef MAGIC_CPP_MACRO_H
#define MAGIC_CPP_MACRO_H

#if __has_include(<source_location>)
#include <source_location>
#endif

// clang-format off
    #if __cplusplus >= 202002L || _MSVC_LANG >= 202002L
        #define MAGIC_CPP_20
    #else 
        error "C++17 or better is required"
    #endif 


    #include <version>
    // clang-format off  
    // fix for msvc-clang, whose function_name will not show the full name of the function


    /**
    * if the compiler supports std::source_location, then use it to get the function name
    * otherwise, use the macro __PRETTY_FUNCTION__ or __FUNCSIG__ to get the function name
    * special case for msvc-clang, whose function_name in sourec_location will not show the full name of the function
    * so we use __FUNCSIG__ to get the function name
    */
    #if __clang__ && _MSC_VER
        #define MAGIC_CPP_FUNCTION_NAME (__FUNCSIG__)

    #elif __cpp_lib_source_location
        #define MAGIC_CPP_FUNCTION_NAME (std::source_location::current().function_name())

    #elif (__clang__ || __GNUC__) && (!_MSC_VER)
        #define MAGIC_CPP_FUNCTION_NAME (__PRETTY_FUNCTION__)

    #elif _MSC_VER
        #define MAGIC_CPP_FUNCTION_NAME (__FUNCSIG__)

    #else
        staic_assert(false, "Unsupported compiler");

    #endif // source_location

// clang-format on
#endif // MAGIC_CPP_MACRO_H