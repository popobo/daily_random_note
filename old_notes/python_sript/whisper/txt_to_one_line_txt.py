import sys

MAX_LINE_TXT_NUM = 64
filename = sys.argv[1]
output_filename = filename + '_one_line.txt'
written_txt_num = 0
with open(filename, 'r', encoding='utf-8') as f_in, open(output_filename, 'w', encoding='utf-8') as f_out:
    for line in f_in:
        line = line.strip()
        new_line = line + ','
        written_txt_num += len(new_line)
        if written_txt_num >= MAX_LINE_TXT_NUM:
            new_line += '\r\n'
            written_txt_num = 0
        f_out.write(new_line)
