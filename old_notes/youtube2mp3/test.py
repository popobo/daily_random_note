import os
from PIL import Image

# 获取当前目录
current_directory = os.getcwd()

# 遍历当前目录下的所有文件
for filename in os.listdir(current_directory):
    if filename.endswith(".jpg"):
        # 打开图片
        image_path = os.path.join(current_directory, filename)
        image = Image.open(image_path)
        
        # 获取原始图片的尺寸
        width, height = image.size
        
        # 调整图片大小为原来的一半
        new_width = width // 2
        new_height = height // 2
        resized_image = image.resize((new_width, new_height))
        
        # 保存调整后的图片
        new_filename = f"resized_{filename}"
        new_image_path = os.path.join(current_directory, new_filename)
        resized_image.save(new_image_path)
        
        print(f"调整图片大小成功：{filename} -> {new_filename}")