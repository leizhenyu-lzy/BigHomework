# 1950083 自动化 刘智宇

# Thirdparty
import numpy as np
import pandas as pd

# USer
import UserToolFunction as user
import DivisionChoiceFunction
from DecisionTreeNodeClass import DecisionTreeNode
from DatasetClass import Dataset

def generateDecisionTree(dataset, current_treenode):
    pass


if __name__ == "__main__":
    # 数据集操作
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.readDataset()
    melon_dataset.countFeaturesPossibleValues()
    melon_dataset.countLabelsPossibleValues()
    melon_dataset.printDatasetInfo()

    # 开始建树
    root_node = DecisionTreeNode(node_num=0)
    generateDecisionTree(dataset=melon_dataset, current_treenode=root_node)


