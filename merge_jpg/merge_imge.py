from PIL import Image
from pathlib import Path

# 获取当前文件夹中的所有图片文件
img_files = [f for f in Path('.').iterdir() if f.is_file() and f.suffix.lower() in ['.jpg', '.jpeg', '.png', '.bmp'] and f.stem.isdigit()]
img_files_sorted = sorted(img_files, key=lambda f: int(f.stem))

if not img_files_sorted:
    print("No image files found in the current directory.")
else:
    # 打开第一张图片，作为拼接后图片的基准大小
    result_img = Image.open(img_files_sorted[0])

    # 记录当前拼接的位置
    current_y = result_img.size[1]

    # 将所有图片拼接起来
    for f in img_files_sorted[1:]:
        img = Image.open(f)

        # 将新图片粘贴到底部
        result_img = result_img.crop((0, 0, result_img.size[0], current_y + img.size[1]))
        result_img.paste(img, (0, current_y))

        # 更新当前拼接的位置
        current_y += img.size[1]

    # 保存拼接后的图片
    result_img.save('result.jpg')
