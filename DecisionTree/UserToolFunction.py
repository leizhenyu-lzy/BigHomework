# 1950083 自动化 刘智宇

# Thirdparty
import os.path
import math

# 对于不同数据集情况需要适当修改（例如数据集特征是否有名称，数据集是否有编号）
dataset_header = 0  # 目前只支持数据集特征有名称，后续可添加
dataset_index_col = None  # 默认是没有index_col

# 常量定义
DatasetsFolder = "Datasets"

DatasetName = "MelonDataset2"
DatasetPath = os.path.join(DatasetsFolder, "MelonDataset2.txt")
DatasetContinuityList = None

# DatasetName = "MelonDataset2alpha"
# DatasetPath = os.path.join(DatasetsFolder, "MelonDataset2alpha.txt")
# DatasetContinuityList = None

# DatasetName = "MelonDataset3"
# DatasetPath = os.path.join(DatasetsFolder, "MelonDataset3.txt")
# DatasetContinuityList = [0, 0, 0, 0, 0, 0, 1, 1]

# DatasetName = "MelonDataset3alpha"
# DatasetPath = os.path.join(DatasetsFolder, "MelonDataset3alpha.txt")
# DatasetContinuityList = [1, 1]

# DatasetName = "IrisDataset"
# DatasetPath = os.path.join(DatasetsFolder, "IrisDataset.csv")
# DatasetContinuityList = [1, 1, 1, 1]
# TrainSamplesAmount = 70

# 全局变量定义
entropy_gain = 0
gain_ratio = 1
gini_index = 2
get_division_feature_method = gini_index  # 注意对于连续值，目前只支持entropy_gain
get_division_feature_methods_name = ["ByEntropyGain", "ByGainRatio", "GainIndex"]

viz_decision_tree_name = DatasetName + get_division_feature_methods_name[get_division_feature_method]


def printSeparateBar(length=40, sign="*"):
    print(sign*length)


def getMaxValueIndex(sequence):
    if type(sequence) == dict:
        key_list = list(sequence.keys())
        value_list = list(sequence.values())
        max_value = max(value_list)
        max_value_index = key_list[value_list.index(max_value)]
        return max_value_index
    elif type(sequence) == list:
        max_value = max(sequence)
        max_value_index = sequence.index(max_value)
        return max_value_index
    else:
        print("Can't handle this sequence.")
        return None

def pLog2p(possibility):
    possibility = float(possibility)
    return possibility * math.log(possibility, 2)


if __name__ == "__main__":
    pass
