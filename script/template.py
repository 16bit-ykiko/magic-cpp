from itertools import product


def generate_template_traits_n(n: int):
    all_combination = []
    for combination in product([0, 1], repeat=n):
        if (len(combination) == 1) or (combination[-1] != combination[-2]):
            all_combination.append(combination)
    code = f"""
template<typename T>
struct template_traits_{n} : std::false_type {{}};
    """

    for combination in all_combination:

        # generate template template parameter
        # e.g. combination = (0, 1), "template <typename, auto...> class T"
        # e.g. combination = (1, 0), "template <auto, typename...> class T"
        # e.g. combination = (0, 0, 1), "template <typename, typename, auto...> class T"
        template_template_params = ""
        for element in combination[:-1]:
            if element == 0:
                template_template_params += "typename, "
            else:
                template_template_params += "auto, "
        template_template_params += "typename..." if combination[-1] == 0 else "auto..."

        # generate template parameter
        # e.g. combination = (0, 1), "typename T1, auto... Ts"
        # e.g. combination = (1, 0), "auto T1, typename... Ts"
        # e.g. combination = (0, 0, 1), "typename T1, typename T2, auto... Ts"
        template_params = ""
        for index, element in list(enumerate(combination, start=1))[:-1]:
            if element == 0:
                template_params += f"typename T{index}, "
            else:
                template_params += f"auto T{index}, "
        template_params += "typename... Ts" if combination[-1] == 0 else "auto... Ts"

        # generate specialization parameter
        # e.g. combination = (0, 1), "T1, Ts..."
        # e.g. combination = (1, 0), "T1, Ts..."
        # e.g. combination = (0, 0, 1), "T1, T2, Ts..."
        specialization_params_code = ""
        for index in range(1, len(combination)):
            specialization_params_code += f"T{index}, "
        specialization_params_code += "Ts..."

        # generate args parameter
        # e.g. combination = (0, 1), "identity<T1>, identity<decltype<Ts>, Ts>..."
        # e.g. combination = (1, 0), "identity<decltype<T1>, T1>, identity<Ts>..."
        # e.g. combination = (0, 0, 1), "identity<T1>, identity<T2>, identity<decltype<Ts>, Ts>..."

        args_code = ""
        for index, element in list(enumerate(combination, start=1))[:-1]:
            if element == 0:
                args_code += f"identity<T{index}>, "
            else:
                args_code += f"identity<decltype(T{index}), T{index}>, "
        if combination[-1] == 0:
            args_code += f"identity<Ts>..."
        else:
            args_code += f"identity<decltype(Ts), Ts>..."

        code += f"""
template <template<{template_template_params}> class U>
constexpr auto template_full_name_{n}()
{{
    return MAGIC_CPP_FUNCTION_NAME;
}}"""

        code += f"""
template <template<{template_template_params}> class T, {template_params}>
struct template_traits_{n}<T<{specialization_params_code}>> : std::true_type 
{{
    constexpr static auto full_name = template_full_name_{n}<T>();
    using args_type = std::tuple<{args_code}>;
}};
    """
    return code


def generate_template_traits(max: int):
    result = ""
    for i in range(1, max + 1):
        result += generate_template_traits_n(i)

    branchs = ""
    for i in range(1, max + 1):
        branchs += f"""
    {"if"if i == 1 else "else if"} constexpr(template_traits_{i}<T>::value)
    {{
        return true;
    }}"""

    result += f"""
template<typename T>
constexpr bool is_template()
{{{branchs}
    else
    {{
        return false;
    }}
}}
"""

    branchs = ""
    for i in range(1, max + 1):
        branchs += f"""
    {"if" if i == 1 else "else if"} constexpr(template_traits_{i}<T>::value)
    {{
        return template_traits_{i}<T>();
    }}"""

    result += f"""
template<typename T>
constexpr auto template_traits_impl() 
{{{branchs}
    else
    {{
        static_assert(is_template<T>(), "not supported");
    }}
}}
"""

    return result

