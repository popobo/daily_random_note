from PIL import Image, ImageDraw, ImageFont

def add_text_to_image(input_image_path, output_image_path, text, position=(50, 50), font_path="arial.ttf", font_size=30, text_color=(0, 0, 0)):
    # 打开图片
    with Image.open(input_image_path) as image:
        # 创建一个可以在给定图像上绘画的对象
        draw = ImageDraw.Draw(image)

        # 设置字体及大小
        font = ImageFont.truetype(font_path, font_size)

        # 将文字绘制到图片上
        draw.text(position, text, fill=text_color, font=font)

        # 保存添加了文字的图片
        image.save(output_image_path)

if __name__ == "__main__":
    input_image_path = "input_image.png"  # 输入图片路径
    output_image_path = "output_image.png"  # 输出图片路径
    text = "AI字幕生成服务 - \n 快速、准确、专业"  # 要添加的文字内容

    # 自定义参数（可选）
    position = (50, 300)  # 文字位置
    font_path = "ZCOOLKuaiLe-Regular.ttf"  # 字体文件路径
    font_size = 50  # 字体大小
    text_color = (100, 100, 100)  # 文字颜色，这里为黑色

    add_text_to_image(input_image_path, output_image_path, text, position, font_path, font_size, text_color)
