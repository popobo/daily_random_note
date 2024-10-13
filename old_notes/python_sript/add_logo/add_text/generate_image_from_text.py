from PIL import Image, ImageDraw, ImageFont

def generate_image_with_text(text_list, image_size, output_image_path, background_color=(255, 255, 255)):
    # 创建一张新的图片
    image = Image.new('RGB', image_size, color=background_color)

    # 创建一个可以在给定图像上绘画的对象
    draw = ImageDraw.Draw(image)

    for text_info in text_list:
        text = text_info['text']
        position = text_info['position']
        font_path = text_info['font_path']
        font_size = text_info['font_size']
        text_color = text_info['text_color']
        center_horizontal = text_info['center_horizontal']
        center_vertical = text_info['center_vertical']

        # 设置字体及大小
        font = ImageFont.truetype(font_path, font_size)

        text_bbox = draw.textbbox(position, text, font=font)
        text_width, text_height = text_bbox[2] - text_bbox[0], text_bbox[3] - text_bbox[1]

        if center_horizontal:
            position = ((image_size[0] - text_width) // 2, position[1])

        if center_vertical:
            position = (position[0], (image_size[1] - text_height) // 2)

        # 将文字绘制到图片上
        draw.text(position, text, fill=text_color, font=font)

    # 保存生成的图片
    image.save(output_image_path)

if __name__ == "__main__":
    output_image_path = "generated_image.png"  # 输出图片路径
    image_size = (512, 512)  # 图片尺寸
    background_color = (255, 255, 255)  # 背景颜色，这里为白色

    text_list = [
        {
            'text': 'AI',
            'position': (0, 10),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 60,
            'text_color': (0, 0, 0),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '视频字幕生成服务',
            'position': (0, 70),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 60,
            'text_color': (0, 0, 0),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '快速、准确、专业',
            'position': (0, 160),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 60,
            'text_color': (222, 0, 0),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '淘宝最低价',
            'position': (0, 220),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 55,
            'text_color': (0, 200, 0),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '无需文稿',
            'position': (0, 310),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 40,
            'text_color': (0, 0, 255),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '支持多种语言',
            'position': (0, 350),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 40,
            'text_color': (0, 0, 180),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '普通话，英语，日语，俄语，法语，阿拉伯语等等',
            'position': (0, 400),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 25,
            'text_color': (0, 0, 100),
            'center_horizontal': True,
            'center_vertical': False
        },
        {
            'text': '一小时视频最快半小时出初稿',
            'position': (0, 440),
            'font_path': 'ZCOOLKuaiLe-Regular.ttf',
            'font_size': 37,
            'text_color': (255, 189, 51),
            'center_horizontal': True,
            'center_vertical': False
        },
    ]

    generate_image_with_text(text_list, image_size, output_image_path, background_color)
