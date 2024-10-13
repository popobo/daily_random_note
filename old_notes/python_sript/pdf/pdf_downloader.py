import requests
from bs4 import BeautifulSoup
import os

def get_web_page_content(url):
    try:
        response = requests.get(url)
        response.raise_for_status()  # Raise an exception if the request was unsuccessful
        return response.text  # Return the content of the web page as text
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")
        return None


def save_content_to_html(content, file_name):
    try:
        with open(file_name, "w", encoding="utf-8") as file:
            file.write(content)
        print(f"Content saved to {file_name} successfully!")
    except IOError as e:
        print(f"Error saving content to {file_name}: {e}")

def download_pdf(url, filename):
    try:
        response = requests.get(url)
        response.raise_for_status()

        with open(filename, "wb") as file:
            file.write(response.content)

        print("PDF file downloaded successfully.")
    except requests.exceptions.HTTPError as errh:
        print(f"HTTP Error occurred: {errh}")
    except requests.exceptions.ConnectionError as errc:
        print(f"Error connecting to the server: {errc}")
    except requests.exceptions.RequestException as err:
        print(f"An error occurred: {err}")

url = "https://www.milesight.cn/documents-download/"

content = get_web_page_content(url)

soup = BeautifulSoup(content, "lxml")

# 传感器
items = soup.find_all(attrs={"data-index":"5"})
pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

# CoWork办公系列
items = soup.find_all(attrs={"data-index":"6"})

pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

# 人数&空间管理系列
items = soup.find_all(attrs={"data-index":"7"})

pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

# 视觉感知系列
items = soup.find_all(attrs={"data-index":"8"})

pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

# 智慧屏幕系列
items = soup.find_all(attrs={"data-index":"8"})

pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

# LoRaWAN控制器
items = soup.find_all(attrs={"data-index":"9"})

pdf_links = []

for item in items:
    links = item.find_all('a')
    for link in links:
        if ("datasheet" in link["href"]):
            pdf_links.append(link["href"])

for link in pdf_links:
    download_pdf(link, "pdfs/" + os.path.basename(link))

