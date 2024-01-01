#ifndef MAGIC_CPP_MACRO_H
#define MAGIC_CPP_MACRO_H

#if __has_include(<source_location>)
#include <source_location>
#endif

#if __cplusplus >= 202002L || _MSVC_LANG >= 202002L
#define MAGIC_CPP_20_SUPPORT
#else

#endif

#include <version>
// fix for msvc-clang, whose function_name will not show the full name of the function

namespace magic
{

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

    inline void unreachable [[noreturn]] ()
    {
        // Uses compiler specific extensions if possible.
        // Even if no extension is used, undefined behavior is still raised by
        // an empty function body and the noreturn attribute.
#if __GNUC__ || __clang__ // GCC, Clang, ICC
        __builtin_unreachable();
#elif _MSC_VER // MSVC
        __assume(false);
#endif
    }
} // namespace magic

#endif // MAGIC_CPP_MACRO_H