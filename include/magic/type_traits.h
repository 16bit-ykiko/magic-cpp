#ifndef MAGIC_CPP_TYPE_TRAITS_H
#define MAGIC_CPP_TYPE_TRAITS_H

#include "function_traits.h"
#include "macro.h"
#include "template_traits.h"

namespace magic
{

    template <typename T>
    struct member_ptr_traits;

    template <typename C, typename M>
    struct member_ptr_traits<M C::*>
    {
        using class_type = C;
        using member_type = M;
    };

    template <typename T>
    using class_type_of_pointer = typename member_ptr_traits<T>::class_type;

    template <typename T>
    using member_type_of_pointer = typename member_ptr_traits<T>::member_type;

} // namespace magic

#endif