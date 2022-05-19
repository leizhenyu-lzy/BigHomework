# 1950083 自动化 刘智宇
import os.path

# 常量定义
DatasetsFolder = "Datasets"
MelonDatasetPath = os.path.join(DatasetsFolder, "MelonDataset.txt")

# 全局变量定义
node_number = 0


def printSeparateBar(length=20, sign="*"):
    print(sign*length)


if __name__ == "__main__":
    printSeparateBar()
