# 
def generate_struct_bind_1():

    for i in range(26):
        vars = []
        for j in range(i+1):
            vars.append(chr(ord('a')+j))
        
        variables_code = ", ".join(vars)
        types_code = ", ".join([f"decltype({var})" for var in vars])
 
        code = f"""else if constexpr (N == {len(vars)}) 
{{
    auto [{variables_code}] = object;
    return Id<std::tuple<{types_code}>>{{}};
}}"""
        print(code)

def generate_struct_bind_2():

    for i in range(26):
        vars = []
        for j in range(i+1):
            vars.append(chr(ord('a')+j))
        
        variables_code = ", ".join(vars)
        code = f"""else if constexpr (N == {len(vars)}) 
{{
    auto&& [{variables_code}] = object;
    return std::get<Index> (std::forward_as_tuple({variables_code}));
}}"""
        print(code)

generate_struct_bind_2()