from PIL import Image, ImageDraw, ImageFont

def convert_white_to_transparent(image_path, output_path):
    # 打开PNG图片
    image = Image.open(image_path)

    # 将图片转换为RGBA模式
    image = image.convert("RGBA")

    # 获取图片的宽度和高度
    width, height = image.size

    # 遍历每个像素点
    for x in range(width):
        for y in range(height):
            # 获取当前像素点的RGBA值
            r, g, b, a = image.getpixel((x, y))

            # 判断当前像素点是否为白色
            #if (r, g, b) == (255, 255, 255):
            if r+g+b >= 700:
                # 将白色像素点的Alpha值设为0，即透明
                image.putpixel((x, y), (r, g, b, 0))

    # 保存为PNG格式的图片
    image.save(output_path, "PNG")

def convert_white_to_complete_white(image_path, output_path):
    # 打开PNG图片
    image = Image.open(image_path)

    # 将图片转换为RGBA模式
    image = image.convert("RGBA")

    # 获取图片的宽度和高度
    width, height = image.size

    # 遍历每个像素点
    for x in range(width):
        for y in range(height):
            # 获取当前像素点的RGBA值
            r, g, b, a = image.getpixel((x, y))

            # 判断当前像素点是否为白色
            #if (r, g, b) == (255, 255, 255):
            if r+g+b >= 700:
                # 将白色像素点的Alpha值设为0，即透明
                image.putpixel((x, y), (255, 255, 255, 255))
    
    # 保存为PNG格式的图片
    image.save(output_path, "PNG")

def add_white_area_with_text(image_path, output_path):
    # 打开PNG图片
    image = Image.open(image_path)

    # 获取图片的宽度和高度
    width, height = image.size

    # 创建新的图片，宽度为原图片宽度加上一块白色区域的宽度，高度与原图片相同
    new_width = width + 300  # 可根据需要调整白色区域的宽度
    new_image = Image.new("RGBA", (new_width, height), (255, 255, 255, 255))

    # 将原图片复制到新图片的左边
    new_image.paste(image, (0, 0))

    # 创建用于绘制文字的draw对象
    draw = ImageDraw.Draw(new_image)

    # 指定使用的字体和字号
    font = ImageFont.truetype("ZCOOLKuaiLe-Regular.ttf", 260)  # 将"path/to/font.ttf"替换为你的字体文件路径，并调整字号

    # 指定文字的起始坐标
    x = width + 20  # 可根据需要调整文字的起始坐标

    # 指定文字的间距
    y = 40  # 可根据需要调整文字的间距

    # 待添加的汉字文本
    text = "汉口"

    # 遍历每个汉字并逐行绘制
    for char in text:
        # 绘制文字
        draw.text((x, y), char, font=font, fill=(0, 0, 0, 255))  # 可根据需要调整文字颜色

        # 调整下一行文字的起始坐标
        y += 290  # 可根据需要调整文字的行间距

    # 保存为PNG格式的图片
    new_image.save(output_path, "PNG")

if __name__ == "__main__":
    add_white_area_with_text("1.png", "3.png")
    convert_white_to_complete_white("3.png", "4.png")
