# 1950083 自动化 刘智宇

# Thirdparty
import pandas as pd

# User
import UserToolFunction as user


"""
Dataset类
"""
class Dataset:
    def __init__(self, dataset_path, dataset_name="MyDataset"):
        self.dataset_name = dataset_name  # 数据集名称
        self.dataset_path = dataset_path  # 数据集文件路径
        self.samples_amount = 0  # 数据集样本个数

        self.data = []  # 数据集内容

        self.features_number = 0  # 特征索引个数
        self.features_name = []  # 全部特征名称，用于后续决策树划分
        self.features = []  # 数据集的全部样本特征数据
        self.features_continuity = []  # 连续特征标记

        self.label_name = ""  # 标签索引名称

        self.features_possible_values = []  # 各个特征的全部可能的特征值，列表内嵌字典
        self.labels_possible_values = {}  # 标签的可能值（字典）

    # 读取数据集
    def readDataset(self, header=0, index_col=None):
        data_frame = pd.read_csv(filepath_or_buffer=self.dataset_path, header=0, index_col=None)  # 读取文件到dataframe
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

        user.printSeparateBar()

    # 统计每一个特征中不充分的值的种类，对应于多元属性
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
        self.labels_possible_values[self.labels[0]] = 1
        for sample_number in range(1, self.samples_amount):
            new_label_value_flag = 1
            temp_label_value = self.labels[sample_number]
            for value in self.labels_possible_values:
                if temp_label_value == value:
                    new_label_value_flag = 0
                    self.labels_possible_values[temp_label_value] += 1
                    break
            if new_label_value_flag == 1:
                self.labels_possible_values[temp_label_value] = 1

    # 统计给定编号的数据的标签值的种类及其个数
    def countLabelsPossibleValuesByList(self, sample_list):
        label_possible_values = {}
        for label_value in self.labels_possible_values:
            label_possible_values[label_value] = 0
        for sample_number in sample_list:
            label_possible_values[self.labels[sample_number]] +=1
        return label_possible_values

    # 判断特征的连续性
    def judgeFeaturesContinuity(self):
        pass


if __name__ == "__main__":
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.readDataset()
    melon_dataset.countFeaturesPossibleValues()
    melon_dataset.countLabelsPossibleValues()
    # melon_dataset.printDatasetInfo()


