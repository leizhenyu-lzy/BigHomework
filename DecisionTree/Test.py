# 1950083 自动化 刘智宇

# Thirdparty
import numpy as np
import pandas as pd
import math

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
    dictionary = melon_dataset.splitSamplesList(division_feature_id=3)
    print(melon_dataset.features_possible_values)

    entropy_gain = melon_dataset.computeOneFeatureEntropyGain(division_feature_id=5, sample_list=[6, 8, 12, 13, 16])
    print(entropy_gain)


