from PIL import Image
from pathlib import Path

# 打开logo
logo = Image.open("logo1.png")

# 计算logo缩放后的大小
max_size = 200  # 假设最大宽或高为100像素
w, h = logo.size
if w > h:
    new_w = max_size
    new_h = int(h / w * max_size)
else:
    new_h = max_size
    new_w = int(w / h * max_size)

# 调整logo大小并保持原来的比例
logo = logo.resize((new_w, new_h), resample=Image.LANCZOS)

# 遍历所有图片并加上logo
for img_file in Path("images").glob("*.jpg"):
    # 打开图片
    image = Image.open(img_file)

    # 将logo粘贴到图片的左上角
    image.paste(logo, (20, 20), mask=logo)

    # 保存结果到指定文件夹
    result_folder = Path("result")
    
    result_file = result_folder.joinpath("logo_" + img_file.name)
    image.save(result_file)
