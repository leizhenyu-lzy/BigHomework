# 1950083 自动化 刘智宇
import pandas as pd
import numpy as np


"""
函数名称：readDataset
函数作用：读取存储在csv文件中的数据集，并返回列表形式的数据内容及其各列名称
函数输入：数据集路径，数据集是否有header(没有写None)，数据集是否有index列(没有写None)
函数输出：二维列表形式数据集dataset、数据集列标签dataset_index(包括label标签)
"""
def readDataset(dataset_name='MelonDataset.txt', dataset_header=0, dataset_index_col=None):
    dataset_df = pd.read_csv(dataset_name, header=dataset_header, index_col=dataset_index_col)
    # 通过values属性转换为列表形式
    dataset = dataset_df.values
    dataset_index = dataset_df.columns.values
    return dataset, dataset_index  # dataset_index包括label名称


"""
函数名称：separateDataLabel
函数作用：将属性和标签拆分，分别返回
函数输入：完整列表形式数据集，包括属性property、标签label(默认只有一个label，且在最后一列)
函数输出：列表形式属性properties、列表形式标签label
"""
def separateDataLabel(complete_dataset):
    (dataset_rows, dataset_cols) = complete_dataset.shape
    properties = complete_dataset[:, 0:dataset_cols-1]
    label = complete_dataset[:, -1]
    return properties, label


"""
函数名称：getAllPropertiesAllValues
函数作用：将数据集每个属性部分的每个可能值进行罗列，方便后续决策树的构建(对于连续属性值的属性种类暂不支持)
函数输入：列表形式数据集属性部分
函数输出：列表形式每个属性的每个可能值
"""
def getPropertiesAllValues(dataset_properties):
    (properties_rows, properties_cols) = dataset_properties.shape
    all_properties_all_vales = []
    for col in range(properties_cols):
        temp_list = [dataset_properties[0][col]]  # 清空 + 添加col列第一个row的值，不可能重复
        for row in range(1, properties_rows):
            append_flag = 1  # 是否被append标志位，默认append
            temp_value = dataset_properties[row][col]
            for value in temp_list:  # 查找temp_list中有无重复
                if temp_value == value:  # 有重复，则不必添加，直接退出
                    append_flag = 0
                    break
            if append_flag == 1:
                temp_list.append(temp_value)
        all_properties_all_vales.append(temp_list)
    return all_properties_all_vales


if __name__ == "__main__":
    melon_dataset, melon_index = readDataset()
    # print(melon_dataset)
    # print(melon_index)
    melon_properties, melon_label = separateDataLabel(melon_dataset)
    # print(melon_properties)
    # print(melon_label)
    result = getPropertiesAllValues(melon_properties)
    for i in range(len(melon_index)-1):
        print(melon_index[i], " : ", result[i])

