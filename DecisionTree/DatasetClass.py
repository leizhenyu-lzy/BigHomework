# 1950083 自动化 刘智宇

# Thirdparty
import pandas as pd

# User
import UserToolFunction as user
from DivisionChoiceFunction import *


"""
Dataset类
"""
class Dataset:
    # __init__
    def __init__(self, dataset_path, dataset_name="MyDataset"):
        self.dataset_name = dataset_name  # 数据集名称
        self.dataset_path = dataset_path  # 数据集文件路径
        self.samples_amount = 0  # 数据集样本个数

        self.data = []  # 数据集内容

        self.features_number = 0  # 特征索引个数
        self.features_name = []  # 全部特征名称，用于后续决策树划分
        self.features = []  # 数据集的全部样本特征数据
        self.features_continuity = []  # 连续特征标记

        self.labels = []
        self.label_name = ""  # 标签索引名称

        self.features_possible_values = []  # 各个特征的全部可能的特征值，列表内嵌字典
        self.labels_possible_values = {}  # 标签的可能值（字典）

        self.available_features_number = []  # not const

    # 高级初始化
    def comprehensiveInitializeDataset(self):
        self.readDataset()
        self.countFeaturesPossibleValues()  #
        self.countLabelsPossibleValues()  #
        self.judgeFeaturesContinuity()  # 确定是否为连续特征

    # 读取数据集
    def readDataset(self, header=0, index_col=None):
        data_frame = pd.read_csv(filepath_or_buffer=self.dataset_path, header=header, index_col=index_col)  # 读取文件到dataframe
        # print(data_frame.dtypes)
        self.data = data_frame.values  # 数据转到列表中
        data_rows, data_cols = self.data.shape
        self.samples_amount = data_rows

        self.features = self.data[:, 0:-1]
        self.features_number = data_cols - 1  # 认为标签只有1列
        self.labels = self.data[:, -1]

        dataset_index_name = data_frame.columns.values  # 取得数据各项索引名称
        self.features_name = dataset_index_name[0:-1]
        self.label_name = dataset_index_name[-1]

        self.available_features_number = [i for i in range(self.features_number)]  # 还没有被决策树用到的特征编号

    # 打印数据集信息
    def printDatasetInfo(self):
        user.printSeparateBar()
        print("dataset_name: ", self.dataset_name, "\n")
        print("dataset_path: ", self.dataset_path, "\n")
        print("sample_numbers: ", self.samples_amount, "\n")

        # print("data:\n", self.data, "\n")

        print("features_number: ", self.features_number, "\n")
        print("features_name:\n", self.features_name, "\n")
        # print("features:\n", self.features, "\n")
        print("features_continuity:\n", self.features_continuity, "\n")

        print("label_name:\n", self.label_name, "\n")
        # print("labels:\n", self.labels, "\n")

        print("features_possible_values:\n", self.features_possible_values, "\n")
        print("labels_possible_values:\n", self.labels_possible_values, "\n")

        print("label_name:\n", self.label_name, "\n")

        user.printSeparateBar()

    # 统计每一个特征中不重复的值的种类，对应于多元特征
    def countFeaturesPossibleValues(self):
        for col in range(self.features_number):
            temp_value_dict = {self.features[0][col]: 1}
            for row in range(1, self.samples_amount):
                new_value_flag = 1  # 判断是否为新值，默认为新值
                current_value = self.features[row][col]  # 当前处理的特征的具体值
                for value in temp_value_dict:
                    if current_value == value:  # 之前出现过该特征值
                        new_value_flag = 0
                        temp_value_dict[value] = temp_value_dict[value] + 1
                        break
                if new_value_flag == 1:  # 没有出现过重复特征值，将其添加
                    temp_value_dict[current_value] = 1
            self.features_possible_values.append(temp_value_dict)

    # 输出self.all_features_all_values
    def showFeaturesPossibleValues(self):
        for cnt in range(self.features_number):
            print(self.features_name[cnt], "\t : \t", self.features_possible_values[cnt])

    # 统计整个数据集的标签值种类及其个数
    def countLabelsPossibleValues(self):
        labels_possible_values = self.countLabelsPossibleValuesByList()
        self.labels_possible_values = labels_possible_values
        # self.labels_possible_values[self.labels[0]] = 1
        # for sample_number in range(1, self.samples_amount):
        #     new_label_value_flag = 1
        #     temp_label_value = self.labels[sample_number]
        #     for value in self.labels_possible_values:
        #         if temp_label_value == value:
        #             new_label_value_flag = 0
        #             self.labels_possible_values[temp_label_value] += 1
        #             break
        #     if new_label_value_flag == 1:
        #         self.labels_possible_values[temp_label_value] = 1

    # 统计给定编号的数据的标签值的种类及其个数
    def countLabelsPossibleValuesByList(self, sample_list=None):
        if sample_list is None:
            sample_list = [cnt for cnt in range(self.samples_amount)]
        labels_possible_values_dict = {self.labels[sample_list[0]]: 1}
        for sample_number in sample_list[1:]:  # 遍历给定样本列表的label
            new_label_value_flag = 1
            temp_label_value = self.labels[sample_number]
            for exist_value in labels_possible_values_dict:
                if temp_label_value == exist_value:
                    new_label_value_flag = 0
                    labels_possible_values_dict[temp_label_value] += 1
                    break
            if new_label_value_flag == 1:
                labels_possible_values_dict[temp_label_value] = 1
        return labels_possible_values_dict
        # label_possible_values = {}  # 字典形式
        # for label_value in self.labels_possible_values:
        #     label_possible_values[label_value] = 0
        # for sample_number in sample_list:
        #     label_possible_values[self.labels[sample_number]] += 1
        # for label_value in self.labels_possible_values:
        #     if label_possible_values[label_value] == 0:
        #         label_possible_values.pop(label_value)
        # return label_possible_values

    # 判断是否所有样本的label都相同
    def judgeSamplesSameCategory(self, sample_list=None):
        label_possible_values = self.countLabelsPossibleValuesByList(sample_list)
        if len(label_possible_values) == 1:
            # print("True ", list(label_possible_values.keys()))
            return True
        else:
            # print("False")
            return False

    # 判断特征的连续性
    def judgeFeaturesContinuity(self, continuity_list=None):
        if continuity_list is None:
            self.features_continuity = [0] * self.features_number  # 默认全部不连续
            return user.success
        elif len(continuity_list) == self.features_number:
            self.features_continuity = continuity_list
            return user.success
        else:
            return user.failure

    # 通过给定特征拆分给定样本列表
    def splitSamplesList(self, division_feature_number, sample_list=None, continuity_split_value=0):
        if sample_list is None:  # 没有给定sample_list，就统计所有值
            sample_list = [cnt for cnt in range(self.samples_amount)]
        split_dict = {}
        if self.features_continuity[division_feature_number]:  # 连续特征
            split_dict['bigger'] = []
            split_dict['smaller'] = []
            # 可能要考虑连续值重复，相等时归给谁
            pass
        else:  # 非连续特征
            for division_feature_value in list(self.features_possible_values[division_feature_number].keys()):
                split_dict[division_feature_value] = []
            for sample_number in sample_list:
                split_dict[self.features[sample_number][division_feature_number]].append(sample_number)
        return split_dict

    # 给定样本列表，计算信息熵
    def computeInformationEntropy(self, sample_list=None):
        information_entropy = 0.0  # 信息熵
        label_values_dict = self.countLabelsPossibleValuesByList(sample_list)  # 统计标签值的字典

        # 得到sample_list长度
        if sample_list is None:
            sum_label_values_number = self.samples_amount
        else:
            sum_label_values_number = len(sample_list)

        label_values_varity = len(label_values_dict)  # 标签值的类别数

        for cnt in range(label_values_varity):
            possibility = (0.0 + list(label_values_dict.values())[cnt]) / sum_label_values_number
            information_entropy -= pLog2p(possibility)
        # print(information_entropy)
        return information_entropy

    # 计算给定的划分特征、样本列表的信息增益
    def computeOneFeatureEntropyGain(self, division_feature_number, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        entropy_gain = 0.0
        # 先计算整体的信息熵
        overall_information_entropy = self.computeInformationEntropy(sample_list)
        entropy_gain += overall_information_entropy
        # 对样本列表根据划分特征进行拆分，得到字典（字典中的value为列表）
        split_dict = self.splitSamplesList(division_feature_number, sample_list)
        # 计算拆分后各样本子列表的加权信息熵，并从信息增益中减去
        for division_feature_value in split_dict:
            sub_information_entropy = self.computeInformationEntropy(split_dict[division_feature_value])
            sub_information_entropy_weight = len(split_dict[division_feature_value])/len(sample_list)
            entropy_gain -= sub_information_entropy * sub_information_entropy_weight
        return entropy_gain

    # 通过信息增益的方式选出最合适的划分特征
    def getDivisionFeatureByEntropyGain(self, available_division_feature_list=None, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_number = 0
        max_entropy_gain = 0.0
        for division_feature_number in available_division_feature_list:
            temp_entropy_gain = self.computeOneFeatureEntropyGain(division_feature_number, sample_list)
            if temp_entropy_gain > max_entropy_gain:
                chosen_division_feature_number = division_feature_number
                max_entropy_gain = temp_entropy_gain
        return chosen_division_feature_number


if __name__ == "__main__":
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.comprehensiveInitializeDataset()

    # melon_dataset.printDatasetInfo()


