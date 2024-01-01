#ifndef MAGIC_CPP_CUSTOMIZED_TYPE_NAME_H
#define MAGIC_CPP_CUSTOMIZED_TYPE_NAME_H
#include "type_tree.h"

template <>
struct magic::type_info<std::size_t>
{
    constexpr static const char* name = "std::size_t";
};

template <>
struct magic::type_info<std::string>
{
    constexpr static const char* name = "std::string";
};

template <>
struct magic::type_info<std::string_view>
{
    constexpr static const char* name = "std::string_view";
};

#endif