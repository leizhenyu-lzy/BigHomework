# 1950083 自动化 刘智宇

# Thirdparty
import math

# User
# import DatasetFunction

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
函数名称：
函数作用：
函数输入：
函数返回：
"""
def computeInformationEntropy(dataset, sample_list, division_feature_number):





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
    # melon_dataset, melon_index = DatasetFunction.readDataset()
    # melon_properties, melon_label = DatasetFunction.separateDataLabel(melon_dataset)
    # all_all = DatasetFunction.getAllPropertiesAllValues(melon_properties)
    # # getEntropyGain(melon_dataset, 5)
    # print(len(all_all[5]))

    print(plog2p(0.5))
