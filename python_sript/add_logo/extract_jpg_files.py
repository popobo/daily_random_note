import os
import shutil

# 定义函数，递归提取文件夹中的jpg图片并复制到指定文件夹
def extract_jpg_files(source_dir, target_dir):
    # 遍历文件夹中的所有文件和子文件夹
    for root, dirs, files in os.walk(source_dir):
        for file in files:
            # 判断文件是否为jpg格式
            if file.lower().endswith(".jpg"):
                # 构造源文件和目标文件的路径
                source_path = os.path.join(root, file)
                target_path = os.path.join(target_dir, file)
                # 复制文件到指定目录
                shutil.copyfile(source_path, target_path)

# 调用函数提取文件夹中的jpg图片并复制到指定文件夹
source_dir = "temp"
target_dir = "images"
extract_jpg_files(source_dir, target_dir)

