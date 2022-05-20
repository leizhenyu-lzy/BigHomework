# 1950083 自动化 刘智宇

# Thirdparty
import numpy as np
import pandas as pd

# USer
import UserToolFunction as user
import DivisionChoiceFunction
from DecisionTreeNodeClass import DecisionTreeNode
from DatasetClass import Dataset

def generateDecisionTree(dataset, current_node):
    # 判断样本是否全属于同一类别
    labels_possible_values_dict = dataset.judgeSamplesSameCategory(sample_list=current_node.include_samples)
    # 全部样本的标签都相同
    if not same_category_flag:
        return
    else:
        current_node.final_label = dataset.labels[current_node.include_samples[0]]



if __name__ == "__main__":
    # 数据集操作
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.comprehensiveInitializeDataset()

    # 开始建树
    root_node = DecisionTreeNode()  # 创建根节点
    root_node.setIncludeSample([cnt for cnt in range(melon_dataset.samples_amount)])  # 树节点包含所有样本
    generateDecisionTree(dataset=melon_dataset, current_node=root_node)


