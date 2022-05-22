# 1950083 自动化 刘智宇
import pandas as pd
import numpy as np

dataset_prefix = 'Datasets/'

"""
函数名称：readDataset
函数作用：读取存储在csv文件中的数据集，并返回列表形式的数据内容及其各列名称
函数输入：数据集路径，数据集是否有header(没有写None)，数据集是否有index列(没有写None)
函数返回：二维列表形式数据集dataset、数据集列标签dataset_index(包括label标签)
"""
def readDataset(dataset_name=dataset_prefix+'MelonDataset.txt', dataset_header=0, dataset_index_col=None):
    dataset_df = pd.read_csv(dataset_name, header=dataset_header, index_col=dataset_index_col)
    # 通过values属性转换为列表形式
    dataset = dataset_df.values
    dataset_index = dataset_df.columns.values
    return dataset, dataset_index  # dataset_index包括label名称


"""
函数名称：separateDataLabel
函数作用：将属性和标签拆分，分别返回
函数输入：完整列表形式数据集，包括属性property、标签label(默认只有一个label，且在最后一列)
函数返回：列表形式属性properties、列表形式标签labels
"""
def separateDataLabel(complete_dataset):
    (dataset_rows, dataset_cols) = complete_dataset.shape
    properties = complete_dataset[:, 0:dataset_cols-1]
    labels = complete_dataset[:, -1]
    return properties, labels


"""
函数名称：getAllPropertiesAllValues
函数作用：将数据集每个属性部分的每个可能值进行罗列，方便后续决策树的构建(对于连续属性值的属性种类暂不支持)
函数输入：列表形式数据集属性部分
函数返回：列表形式每个属性的每个可能值
"""
def getAllPropertiesAllValues(dataset_properties):
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


"""
函数名称：showAllPropertiesAllValues
函数作用：将数据集每一个属性，及对应属性每一个可能值进行打印
函数输入：列表形式每个属性的每个可能值，列表形式数据集
函数返回：无
"""
def showAllPropertiesAllValues(all_properties_all_values, dataset_index):
    for cnt in range(len(all_properties_all_values)):
        print(dataset_index[cnt], " : ", all_properties_all_values[cnt])


"""
函数名称：getOnePropertyAllValues
函数作用：将数据集某一个属性部分的每个可能值进行罗列(对于连续属性值的属性种类暂不支持)
函数输入：列表形式数据集某个属性的所有值
函数返回：列表形式某个属性的全部可能值
"""
def getOnePropertyAllValues(one_col_property):
    rows = len(one_col_property)
    one_property_all_vales = [one_col_property[0]]
    for row in range(1, rows):
        append_flag = 1  # 是否被append标志位，默认append
        temp_value = one_col_property[row]
        for value in one_property_all_vales:  # 查找temp_list中有无重复
            if temp_value == value:  # 有重复，则不必添加，直接退出
                append_flag = 0
                break
        if append_flag == 1:
            one_property_all_vales.append(temp_value)
    return one_property_all_vales


"""
函数名称：showOnePropertyAllValues
函数作用：将数据集某一个属性，及其每一个可能值进行打印
函数输入：列表形式某一个属性的每个可能值，属性名
函数返回：无
"""
def showOnePropertyAllValues(one_property_all_vales, property_name):
    print(property_name, " : ", one_property_all_vales)


"""
函数名称：judgePropertiesContinuity
函数作用：
函数输入：
函数返回：
"""
def judgePropertiesContinuity(all_properties_all_values, sample_numbers):




if __name__ == "__main__":
    melon_dataset, melon_index = readDataset()
    # print(melon_dataset)
    # print(melon_index)
    melon_properties, melon_label = separateDataLabel(melon_dataset)
    # print(melon_properties)
    # print(melon_label)
    # result = getAllPropertiesAllValues(melon_properties)
    # showAllPropertiesAllValues(result, melon_index)
    one_all = getOnePropertyAllValues(melon_properties[:, 1])
    showOnePropertyAllValues(one_all, melon_index[1])

