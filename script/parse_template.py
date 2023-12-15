# used to generate the raw_name_of function
# the n is template parameter number

def generate_raw_name(n: int):
    all_combination = []
    for i in range(1, 2 ** n - 1):
        combination = []
        for j in range(n):
            combination.append((i >> j) & 1)
        all_combination.append(combination)

    for combination in all_combination:
        template_template_params_code = ", ".join(
            ["typename" if element == 0 else "auto" for element in combination])

        template_params = [f"{'typename' if element == 0 else 'auto'} T{index}" for index,
                           element in enumerate(combination, start=1)]
        template_params_code = ", ".join(template_params)

        specialization_params = [f"T{index}" for index,
                                 _ in enumerate(combination, start=1)]
        specialization_params_code = ", ".join(specialization_params)

        parse_code = "\n".join([f"{'ADD_TYPE(' if element == 0 else 'ADD_NTTP('}T{index})" for index,
                                element in enumerate(combination, start=1)])

        text = f"""
template <template <{template_template_params_code}> class T, {template_params_code}>
struct type_traits<T<{specialization_params_code}>>
{{
PARSE_START
{parse_code}
PARSE_END
}};"""
        print(text)


generate_raw_name(2)
generate_raw_name(3)
generate_raw_name(4)
