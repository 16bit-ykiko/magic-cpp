from itertools import product


def generate_all_combination(n: int):
    # generate all combination of 0 and 1, and remove the if the last two elements are the same
    # e.g. n = 2, all_combination = [(0, 1), (1, 0)]
    # e.g. n = 3, all_combination = [(0, 1), (1, 0), (0, 0, 1), (0, 1, 0), (1, 0, 1), (1, 1, 0)]

    all_combination = [(0,), (1,)]
    for num in range(1, n + 1):
        temp = list(product([0, 1], repeat=num))[1:-1]
        for element in temp:
            if (element[-1] != element[-2]):
                all_combination.append(element)
    return all_combination


def generate_template_parse(n: int):
    all_combination = generate_all_combination(n)
    text = ""

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
        # e.g. combination = (0, 1), "typename T1, auto... T2"
        # e.g. combination = (1, 0), "auto T1, typename... T2"
        # e.g. combination = (0, 0, 1), "typename T1, typename T2, auto... T3"
        template_params = ""
        for index, element in list(enumerate(combination, start=1))[:-1]:
            if element == 0:
                template_params += f"typename T{index}, "
            else:
                template_params += f"auto T{index}, "
        template_params += "typename... Ts" if combination[-1] == 0 else "auto... Ts"

        # generate requires to avoid ambiguous
        requires = "requires (sizeof...(Ts) > 0)" if len(
            combination) > 1 else ""

        # generate specialization parameter
        # e.g. combination = (0, 1), "T1, Ts..."
        # e.g. combination = (1, 0), "T1, Ts..."
        # e.g. combination = (0, 0, 1), "T1, T2, Ts..."
        specialization_params_code = ""
        for index in range(1, len(combination)):
            specialization_params_code += f"T{index}, "
        specialization_params_code += "Ts..."

        # generate parse code
        # e.g. combination = (0, 1)
        # MAGIC_ADD_TYPE(T1)
        # MAGIC_ADD_VARADIC_NTTPS(Ts)
        #
        # e.g. combination = (1, 1, 0)
        # MAGIC_ADD_NTTP(T1)
        # MAGIC_ADD_NTTP(T2)
        # MAGIC_ADD_VARADIC_TYPES(Ts)

        parse_code = ""
        for index, element in list(enumerate(combination, start=1))[:-1]:
            if element == 0:
                parse_code += f"    MAGIC_ADD_TYPE(T{index})\n"
            else:
                parse_code += f"    MAGIC_ADD_NTTP(T{index})\n"
        if combination[-1] == 0:
            parse_code += f"    MAGIC_ADD_VARADIC_TYPES(Ts)"
        else:
            parse_code += f"    MAGIC_ADD_VARADIC_NTTPS(Ts)"

        text += f"""
template <template <{template_template_params}> class T, {template_params}> {requires}
struct type_traits<T<{specialization_params_code}>> 
{{
    MAGIC_TEMPLATE_NAME
    MAGIC_PARSE_START
{parse_code}
    MAGIC_PARSE_END
}};
"""
    return text
