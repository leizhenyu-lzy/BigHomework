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
    melon_dataset.printDatasetInfo()
    # melon_dataset.showFeaturesPossibleValues()

    feature_number = melon_dataset.getDivisionFeatureByEntropyGain()

    melon_dataset.judgeSamplesSameCategory([0 for i in range(5)])

    # melon_dataset.splitSamplesList(0)



