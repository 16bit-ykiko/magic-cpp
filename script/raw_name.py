# used to generate the raw_name_of function
# the n is template parameter number

def generate_raw_name(n: int):
    all_combination = []
    for i in range(1, 2 ** n - 1):
        combination = []
        for j in range(n):
            combination.append((i >> j) & 1)
        all_combination.append(combination)

    for lt in all_combination:
        index = 0
        text = ", ".join(
            ["typename" if element == 0 else "auto" for element in lt])
        text = f"template <template <{text}> class T>"
        text += "\nconstexpr auto raw_name_of(){METAINFO};\n"
        print(text)


generate_raw_name(2)
generate_raw_name(3)
generate_raw_name(4)
