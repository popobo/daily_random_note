import pandas as pd

def find_missing_names(file_path):
    # 读取Excel文件
    df = pd.read_excel(file_path)
    
    # 将F列和H列的人名转换为集合，以方便进行差集计算
    f_names = set(df.iloc[:, 5].dropna())
    h_names = set(df.iloc[:, 7].dropna())

    # 找出在H列中存在，但在F列中缺少的人名
    missing_names = f_names - h_names
    
    return missing_names

# 用你的Excel文件路径替换'your_file_path.xlsx'
file_path = 'test.xlsx'
missing_names = find_missing_names(file_path)

print("在H列中存在，但在F列中缺少的人名：")
for name in missing_names:
    print(name)