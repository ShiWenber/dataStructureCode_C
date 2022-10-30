# 生成1000行4列,用逗号分隔的随机数并写入csv文件
import random
res = []
for i in range(1000):
    res.append([random.randint(1, 100), random.randint(1, 100), random.randint(1, 100), random.randint(1, 100)])
with open('random_table.csv', 'w') as f:
    for i in res:
        f.write(','.join([str(j) for j in i]) + '\n')
    f.close()

    