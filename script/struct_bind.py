# the generate_struct_bind accept a number of max fields, and generate the the struct bind of it
# for example, if the max is 2, then the code will be like this:

# :param max: the max number of fields that you want to support
# :return: the final code of generation

#   if constexpr (N == 0)
#   {
#       return std::type_identity<std::tuple<>>{};
#   }
#   else if constexpr (N == 1)
#   {
#       auto [var0] = object;
#       return std::type_identity<std::tuple<decltype(var0)>>{};
#   }
#   else if constexpr (N == 2)
#   {
#       auto [var0, var1] = object;
#       return std::type_identity<std::tuple<decltype(var0), decltype(var1)>>{};
#   }
#   else
#   {
#       static_assert(N <= 2, "the max of supported fields is 2");
#   }

def generate_struct_bind_of_field_types(max: int):

    # handle the case of 0
    code = f"""
if constexpr (N == 0)
{{
    return std::type_identity<std::tuple<>>{{}};
}}"""

    for num in range(max):
        vars = []
        for index in range(num + 1):
            vars.append(f"v{index}")

        variables = ", ".join(vars)
        # variables just like: v0, v1, v2, ...

        types: str = ", ".join([f"decltype({var})" for var in vars])
        # types just like: decltype(v0), decltype(v1), decltype(v2), ...

        code += f"""
else if constexpr (N == {len(vars)}) 
{{
    auto [{variables}] = object;
    return std::type_identity<std::tuple<{types}>>{{}};
}}"""

    # handle the case of N > max
    code += f"""
else
{{
    static_assert(N <= {max}, "the max of supported fields is {max}");
}}"""
    return code


def generate_struct_bind_of_field_access(max: int):
    code = f"""if constexpr (N == 0)
{{
    static_assert(N != 0, "the object has no fields");
}}"""

    for num in range(max):
        vars = []
        for index in range(num+1):
            vars.append(f"v{index}")

        variables = ", ".join(vars)
        # variables just like: v0, v1, v2, ...

        code += f"""
else if constexpr (N == {len(vars)}) 
{{
    auto&& [{variables}] = object;
    return std::get<Index> (std::forward_as_tuple({variables}));
}}"""

    code += f"""
else
{{
    static_assert(N <= {max}, "the max of supported fields is {max}");
}}"""
    return code
