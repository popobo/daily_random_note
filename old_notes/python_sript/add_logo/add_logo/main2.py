from PIL import Image
from pathlib import Path

# 打开 logo
logo = Image.open("logo1.png")

# 指定 logo 占据图片的比例
logo_ratio = 0.15  # 假设 logo 占据图片宽度的 20%

# 指定 logo 与图片边缘的距离比例
margin_ratio = 0.02  # 假设 logo 与图片边缘的距离占图片宽度的 5%

# 遍历所有图片并加上 logo
for img_file in Path("images").glob("*.jpg"):
    # 打开图片
    image = Image.open(img_file)

    # 计算图片的大小
    img_w, img_h = image.size

    # 根据图片大小和指定比例计算 logo 的新尺寸
    new_w = int(img_w * logo_ratio)
    new_h = int(logo.height * (new_w / logo.width))

    # 调整 logo 大小并保持原来的比例
    resized_logo = logo.resize((new_w, new_h), resample=Image.LANCZOS)

    # 计算 logo 与图片边缘的距离
    margin_x = int(img_w * margin_ratio)
    margin_y = int(img_h * margin_ratio)

    # 将 logo 粘贴到图片的左上角，并根据比例调整间距
    image.paste(resized_logo, (margin_x, margin_y), mask=resized_logo)

    # 保存结果到指定文件夹
    result_folder = Path("result")
    
    result_file = result_folder.joinpath("logo_" + img_file.name)
    image.save(result_file)
