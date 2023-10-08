def create_set_from_file(file_path):
    result_set = set()
    
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            # 去除每行首尾的空白字符，并将整行作为一个元素添加到集合中
            line_data = line.strip()
            result_set.add(line_data)
    
    return result_set

def create_list_from_file(file_path):
    result_list = []
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            # 去除每行首尾的空白字符，并将整行作为一个元素添加到集合中
            line_data = line.strip()
            result_list.append(line_data)
    return result_list

# sets_1 = create_set_from_file('f_file')
# sets_2 = create_set_from_file('h_file')

# miss_sets = sets_1 - sets_2

# 打印每一行生成的集合
# for i, line_set in enumerate(miss_sets, 1):
#     print(f"Line {i}: {line_set}")


list_f = create_list_from_file('f_file')
list_h = create_list_from_file('h_file')

diff = []

for f in list_f:
    flag = False
    for h in list_h:
        if f == h:
            flag = True
    if not flag:
        diff.append(f)

print(diff)    
