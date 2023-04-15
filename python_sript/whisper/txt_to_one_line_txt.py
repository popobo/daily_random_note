with open('video1.txt', 'r', encoding='utf-8') as f_in, open('output.txt', 'w', encoding='utf-8') as f_out:
    for line in f_in:
        line = line.strip()  # 去除行末尾的换行符
        new_line = line + ','  # 在行末添加逗号
        f_out.write(new_line)  # 写入到输出文件中
