import pdfplumber
from openpyxl import Workbook
import os
import re

except_file = ["ds7610-datasheet-cn.pdf", "ws302-datasheet-cn.pdf", "wts-series-datasheet-cn.pdf", "ws50x-datasheet-cn.pdf", "vs340&vs341-datasheet-cn.pdf"]

directory = "pdfs"
text_directory = "texts"
parsed_text_directory = "parsed_text"
table_directory = "table"

def list_to_string(str_list : list[str]):
    result = ""
    for line in str_list:
        result = result + line + "\n"
    return result

def create_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

files = os.listdir(directory)

def parse_pdf():
    create_directory(text_directory)
    create_directory(parsed_text_directory)
    for file in files:
            if file in except_file:
                continue

            file_path = os.path.join(directory, file)
            text_file = text_directory + "/" + os.path.splitext(file)[0]
            parsed_text_file = parsed_text_directory + "/" + os.path.splitext(file)[0]
            lines = []
            with open(text_file, "w+", encoding="utf8") as text, open(parsed_text_file, "w+", encoding="utf8") as parsed_text:
                with pdfplumber.open(file_path) as pdf:
                    for page in pdf.pages:  
                        lines += page.extract_text().splitlines()

                pattern = r"^[2&]$"
                filter_lines = []
                for line in lines:
                    match = re.search(pattern, line)
                    if not match:
                        filter_lines.append(line.replace(" ", "").replace(" ", ""))
                
                sensor_name = filter_lines[1].replace(" ", "")
                sensor_model = filter_lines[2].replace(" ", "")
                print(sensor_name)
                print(sensor_model)

                index = 0
                highlight_index = 0
                standard_index = 0
                for line in filter_lines:
                    if line == "产品亮点":
                        highlight_index = index
                    if line == "产品规格":
                        standard_index = index
                    index = index + 1
                    text.write(line + "\n")

                introduction = list_to_string(filter_lines[highlight_index + 1: standard_index])
                print(introduction)
                parsed_text.write("产品名称：" + sensor_name + "\n")
                parsed_text.write("产品型号：" + sensor_model + "\n")
                parsed_text.write("产品亮点：\n" + introduction)


class TreeNode:
    def __init__(self, data):
        self.data = data
        self.children = []

    def add_child(self, child):
        if isinstance(child, TreeNode):
            self.children.append(child)
        else:
            raise TypeError("Child must be an instance of TreeNode")

    def remove_child(self, child):
        if child in self.children:
            self.children.remove(child)
        else:
            raise ValueError("Child not found")

    def get_children(self):
        return self.children

    def __str__(self):
        return str(self.data)

class table_item:
    def __init__(self) -> None:
        self.first_row_index = 0
        self.second_row_index = []
        self.third_row_index = []
    

def parse_pdf_table():
    create_directory(table_directory)
    for file in files:
        if file in except_file:
            continue
        table = []
        file_path = os.path.join(directory, file)
        table_file_name = table_directory + "/" + os.path.splitext(file)[0]
        parsed_table_file_name = parsed_text_directory + "/" + os.path.splitext(file)[0]
        with open(table_file_name, "w+", encoding="utf8") as table_file:
            with pdfplumber.open(file_path) as pdf:
                for page in pdf.pages:
                    if page.extract_table() is not None:
                        table = table + page.extract_table()
            for item in table:
                table_file.write(str(item))
                table_file.write("\n")
        
        devices = []
        for item in table[0]:
            if item is not None and item != "型号":
                devices.append(item)

        first_row_index = []
        second_row_index = []
        third_row_index = []
        row_index = 1
        for table_line in table[1:]:
            col_index = 0
            for item in table_line:
                if col_index == 0 and item is not None:
                    first_row_index.append(row_index)
                    break
                elif col_index == 1 and item is not None:
                    second_row_index.append(row_index)
                    break
                elif col_index == 2 and item is not None:
                    third_row_index.append(row_index)
                    break
                col_index = col_index + 1
            row_index = row_index + 1

        result = []
        len_f = len(first_row_index)
        len_s = len(second_row_index)
        len_t = len(third_row_index)
        for i in range(len_f):
            fi = first_row_index[i]
            item = table_item()
            item.first_row_index = fi
            for j in range(len(second_row_index)):
                si = second_row_index[j]
                if fi < si and ((i == len_f - 1) or (si < first_row_index[i + 1])):
                    item.second_row_index.append(si)
                for k in range(len_t):
                    ti = third_row_index[k]
                    if si < ti and ((j == len_s - 1) or (ti < second_row_index[j + 1])):
                        item.third_row_index.append(ti)
            result.append(item)
                
        print("-----------------")
        # print(str(devices))
        for i in range(len(devices)):
            standard = ""
            for j in result:
                if len(j.third_row_index) > 0:
                    """
                        采集内容：
                            温度：
                            采集范围：-20°C - 60°C
                    """
                    standard = standard + table[j.first_row_index][0] + "：\n"
                    standard = standard + " " + table[j.first_row_index][1] + "：\n"
                    standard = standard + "  " + table[j.first_row_index][2] + "："
                    standard = standard + table[j.first_row_index][3 + i]
                    print(standard)
                    

def parse_pdf_table_new():
    create_directory(table_directory)
    for file in files:
        if file in except_file:
            continue
        table = []
        file_path = os.path.join(directory, file)
        table_file_name = table_directory + "/" + os.path.splitext(file)[0]
        parsed_table_file_name = parsed_text_directory + "/" + os.path.splitext(file)[0]
        with open(table_file_name, "w+", encoding="utf8") as table_file:
            with pdfplumber.open(file_path) as pdf:
                for page in pdf.pages:
                    if page.extract_table() is not None:
                        table = table + page.extract_table()
            for item in table:
                table_file.write(str(item))
                table_file.write("\n")

        for i in range(len(table[1:])):
            if table[i][0] is not None:
                for i in range(len(table[i:])):
                    pass

# parse_pdf()
parse_pdf_table_new()