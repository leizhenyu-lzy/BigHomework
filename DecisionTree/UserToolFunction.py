# 1950083 自动化 刘智宇

# Thirdparty
import os.path
import math


# 常量定义
DatasetsFolder = "Datasets"
MelonDatasetPath = os.path.join(DatasetsFolder, "MelonDataset.txt")
TestDatasetPath = os.path.join(DatasetsFolder, "TestDataset.txt")


success = 1
failure = 0


# 全局变量定义
entropy_gain = 0
gain_ratio = 1
gini_index = 2
get_division_feature_method = gini_index
get_division_feature_methods_name = ["ByEntropyGain", "ByGainRatio", "GainIndex"]


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
    printSeparateBar()
