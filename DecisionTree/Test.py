# 1950083 自动化 刘智宇

# Thirdparty
import numpy as np
import pandas as pd

# USer
import UserToolFunction as user
import DivisionChoiceFunction as DCF
from DecisionTreeNodeClass import DecisionTreeNode
from DatasetClass import Dataset

"""
函数名称：
函数作用：
函数输入：
函数返回：
"""

if __name__ == "__main__":
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.comprehensiveInitializeDataset()

















    # filename = os.path.join("Datasets", "IncompleteDataset.txt")
    # df = pd.read_csv(filename, header=None, index_col=None)
    # dfv = df.values
    # print(dfv)
    # rows, cols = dfv.shape
    # # print(len(dfv[1]))
    # for row in range(rows):
    #     counter = 0
    #     for col in range(cols):
    #         element = dfv[row, col]
    #         if not pd.isna(element):
    #             counter = counter+1
    #     print(counter)
