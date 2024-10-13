import os
import requests
from markdown_it import MarkdownIt

# 初始化markdown-it解析器
md = MarkdownIt()
md_path = "nextjs_chapter_2.md"
save_dir = "images"  # 图片保存目录

# 解析Markdown文件的函数
def extract_image_links_and_aliases(markdown_content):
    tokens = md.parse(markdown_content)
    images = []
    
    # 遍历tokens，查找图片相关的信息
    for token in tokens:
        if token.type == 'inline' and token.children:
            for child in token.children:
                if child.type == 'image':
                    images.append({
                        'alias': child.content,  # 图片别名
                        'link': child.attrs.get('src')  # 图片链接
                    })
    return images

# 根据Content-Type确定图片后缀
def get_image_extension(content_type):
    if 'image/jpeg' in content_type:
        return '.jpg'
    elif 'image/png' in content_type:
        return '.png'
    elif 'image/gif' in content_type:
        return '.gif'
    elif 'image/svg+xml' in content_type:
        return '.svg'
    else:
        return ''  # 如果无法识别格式

# 下载图片到本地并添加后缀
def download_image(image_url, save_dir, index):
    try:
        # 请求图片资源
        response = requests.get(image_url, stream=True)
        if response.status_code == 200:
            # 获取图片的Content-Type
            content_type = response.headers.get('Content-Type', '')
            # 根据Content-Type确定图片后缀
            extension = get_image_extension(content_type)
            
            # 如果没有确定到文件扩展名，使用默认的扩展名
            if not extension:
                print(f"Warning: Unrecognized image type for {image_url}")
                extension = '.jpg'  # 使用默认扩展名
            
            # 获取文件名并加上后缀
            file_name = md_path + "_" + str(index)  # 去除可能的query参数
            if not file_name.endswith(extension):
                file_name += extension
            
            local_path = os.path.join(save_dir, file_name)
            
            # 保存图片
            with open(local_path, 'wb') as f:
                for chunk in response.iter_content(1024):
                    f.write(chunk)
            
            return local_path
        else:
            print(f"Failed to download {image_url}")
            return None
    except Exception as e:
        print(f"Error downloading {image_url}: {e}")
        return None

# 替换Markdown内容中的图片链接为本地链接
def replace_image_links(markdown_content, image_data, save_dir):
    updated_content = markdown_content
    index = 0
    for img in image_data:
        local_path = download_image(img['link'], save_dir, index)
        if local_path:
            # 替换Markdown内容中的图片链接为本地链接
            updated_content = updated_content.replace(img['link'], local_path)
        index = index + 1
    return updated_content

# 创建保存图片的目录
if not os.path.exists(save_dir):
    os.makedirs(save_dir)

# 读取Markdown文件内容
with open(md_path, 'r', encoding='utf-8') as f:
    markdown_content = f.read()

# 解析并提取图片信息
image_data = extract_image_links_and_aliases(markdown_content)

# 替换Markdown内容中的图片链接
updated_markdown_content = replace_image_links(markdown_content, image_data, save_dir)

# 将更新后的Markdown内容保存到文件
updated_md_path = md_path
with open(updated_md_path, 'w', encoding='utf-8') as f:
    f.write(updated_markdown_content)

print(f"Updated Markdown saved as {updated_md_path}")
