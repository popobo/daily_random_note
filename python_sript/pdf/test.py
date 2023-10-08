class test:
    value = []

result = []

for i in range(3):
    item = test()
    for j in range(2):
        item.value.append(i)
    result.append(item)

print(result)