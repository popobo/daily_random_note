input_file = 'one_line.txt'  # 输入文件名
output_file = 'output.txt'  # 输出文件名

with open(input_file, 'r', encoding='utf-8') as f:
    content = f.read()

split_content = [content[i:i+2048] for i in range(0, len(content), 2048)]
formatted_content = '\n\n\n'.join(split_content)

with open(output_file, 'w', encoding='utf-8') as f:
    f.write(formatted_content)
