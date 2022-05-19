# 1950083 自动化 刘智宇

# Thirdparty
import math

# User
# import DatasetFunction
from DatasetClass import Dataset

"""
函数名称：pLog2p
函数作用：计算p*log2(p),便于计算信息熵以及信息增益
函数输入：概率p
函数返回：p*log2(p)的值
"""
def pLog2p(possibility):
    possibility = float(possibility)
    return possibility * math.log(possibility, 2)


"""
函数名称：computeInformationEntropy
函数作用：给定数据集和样本列表，归类并计算
函数输入：
函数返回：
"""
def computeInformationEntropy(dataset, sample_list=None):
    information_entropy = 0.0  # 信息熵
    label_values_dict = dataset.countLabelsPossibleValuesByList(sample_list)  # 统计标签值的字典

    # 得到sample_list长度
    if sample_list is None:
        sum_label_values_number = dataset.samples_amount
    else:
        sum_label_values_number = len(sample_list)

    label_values_varity = len(label_values_dict)  # 标签值的类别数

    for cnt in range(label_values_varity):
        possibility = 0.0 + list(label_values_dict.values())[cnt]/sum_label_values_number
        information_entropy -= pLog2p(possibility)
    # print(information_entropy)
    return information_entropy



"""
函数名称：
函数作用：
函数输入：
函数返回：
"""
def computeOneFeatureEntropyGain(dataset, sample_list):
    # 先计算整个dataset的信息熵

    # 再计算根据给定特征进行分类后各个子dataset的加权信息熵

    information_gain = 0.0
    return information_gain


"""
函数名称：computeOneFeatureGiniIndex
函数作用：计算一个特征的基尼系数
函数输入：数据集和选择的特征序号
函数返回：计算出的基尼系数值
"""
def computeOneFeatureGiniIndex(dataset, chosen_feature_number):
    pass


"""
函数名称：
函数作用：
函数输入：
函数返回：
"""
def computeOneFeatureGainRatio():
    pass


if __name__ == "__main__":
    pass

