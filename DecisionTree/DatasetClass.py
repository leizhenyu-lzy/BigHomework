# 1950083 自动化 刘智宇

# Thirdparty
import pandas as pd

# User
import UserToolFunction as user


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

        self.features_possible_values = []  # 各个特征的全部可能的特征值，列表元素为字典，字典内为feature的值及对应个数
        self.labels_possible_values = {}  # 标签的可能值（字典,labels可能值和对应数量）

        # self.available_features_number_list = []  # not const

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

        # self.available_features_number_list = [i for i in range(self.features_number)]  # 还没有被决策树用到的特征编号

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
                        temp_value_dict[value] += 1
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
        # if len(sample_list) == 0:
        #     print("empty")
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

    # # 判断是否所有样本的label都相同
    # def judgeSamplesBelongSameCategory(self, sample_list=None):
    #     label_possible_values = self.countLabelsPossibleValuesByList(sample_list)
    #     if len(label_possible_values) == 1:
    #         # print("True ", list(label_possible_values.keys()))
    #         return True
    #     else:
    #         # print("False")
    #         return False

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
    def splitSamplesList(self, division_feature_id, sample_list=None, continuity_split_value=0):
        if sample_list is None:  # 没有给定sample_list，就统计所有值
            sample_list = [cnt for cnt in range(self.samples_amount)]
        split_dict = {}
        if self.features_continuity[division_feature_id]:  # 连续特征
            split_dict['bigger'] = []
            split_dict['smaller'] = []
            # 可能要考虑连续值重复，相等时归给谁
            pass
        else:  # 非连续特征
            for division_feature_value in list(self.features_possible_values[division_feature_id].keys()):
                split_dict[division_feature_value] = []
            for sample_number in sample_list:
                split_dict[self.features[sample_number][division_feature_id]].append(sample_number)
            for division_feature_value in list(self.features_possible_values[division_feature_id].keys()):  # 避免空的列表返回
                if len(split_dict[division_feature_value]) == 0:
                    split_dict.pop(division_feature_value)  # 删除值为空列表的键值对
        return split_dict

    # 给定样本列表，计算信息熵
    def computeInformationEntropy(self, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]

        information_entropy = 0.0  # 信息熵
        label_values_dict = self.countLabelsPossibleValuesByList(sample_list)  # 统计标签值的字典

        for label_value in label_values_dict:
            label_proportion = (0.0 + label_values_dict[label_value])/len(sample_list)
            information_entropy -= user.pLog2p(label_proportion)
        # print(information_entropy)
        return information_entropy

    # 计算给定的划分特征、样本列表的信息增益
    def computeOneFeatureEntropyGain(self, division_feature_id, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        entropy_gain = 0.0
        # 先计算整体的信息熵
        overall_information_entropy = self.computeInformationEntropy(sample_list)
        entropy_gain += overall_information_entropy
        # 对样本列表根据划分特征进行拆分，得到字典（字典中的value为列表）
        split_dict = self.splitSamplesList(division_feature_id, sample_list)
        # 计算拆分后各样本子列表的加权信息熵，并从信息增益中减去
        for division_feature_id in split_dict:
            sub_information_entropy = self.computeInformationEntropy(split_dict[division_feature_id])  # 计算划分后的子样本集的信息熵
            sub_information_entropy_weight = len(split_dict[division_feature_id])/len(sample_list)  # 计算子样本集相应权重（子样本集的占比）
            entropy_gain -= sub_information_entropy * sub_information_entropy_weight
        return entropy_gain

    # 通过信息增益的方式选出最合适的划分特征
    def selectDivisionFeatureByEntropyGain(self, available_division_feature_list=None, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = 0
        max_entropy_gain = 0.0
        for division_feature_id in available_division_feature_list:
            temp_entropy_gain = self.computeOneFeatureEntropyGain(division_feature_id, sample_list)
            if temp_entropy_gain > max_entropy_gain:
                chosen_division_feature_id = division_feature_id
                max_entropy_gain = temp_entropy_gain
        return chosen_division_feature_id

    # 计算某一特征的固有值intrinsic value
    def computeIntrinsicValue(self, division_feature_id, sample_list=None):
        intrinsic_value = 0.0
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        split_dict = self.splitSamplesList(division_feature_id, sample_list)
        for division_feature in split_dict:
            sub_samples_proportion = len(split_dict[division_feature])/len(sample_list)
            intrinsic_value -= user.pLog2p(sub_samples_proportion)
        # print("intrinsic_value : ", intrinsic_value)
        return intrinsic_value
    
    # 计算某一特征的信息增益率
    def computeOneFeatureGainRatio(self, division_feature_id, sample_list=None):
        feature_intrinsic_value = self.computeIntrinsicValue(division_feature_id, sample_list)  # 计算特征的固有值
        feature_information_gain = self.computeOneFeatureEntropyGain(division_feature_id, sample_list)  # 计算特征的信息增益
        return feature_information_gain/feature_intrinsic_value  # 计算特征增益率

    # 通过信息增益率的方式选出最合适的划分特征
    def selectDivisionFeatureByGainRatio(self, available_division_feature_list=None, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = 0
        max_gain_ratio = 0.0
        for division_feature_id in available_division_feature_list:
            temp_gain_ratio = self.computeOneFeatureGainRatio(division_feature_id, sample_list)
            # print(temp_gain_ratio)
            if temp_gain_ratio > max_gain_ratio:
                chosen_division_feature_id = division_feature_id
                max_gain_ratio = temp_gain_ratio
        return chosen_division_feature_id

    # 计算基尼值
    def computeGiniValue(self, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部样本
            sample_list = [cnt for cnt in range(self.samples_amount)]

        gini_value = 1.0  # 使用1-[sigma(p)]^2的公式进行基尼值的度量，故从1.0开始减
        label_values_dict = self.countLabelsPossibleValuesByList(sample_list)  # 统计标签值的字典

        for label_value in label_values_dict:
            label_proportion = (0.0 + label_values_dict[label_value])/len(sample_list)
            gini_value -= pow(label_proportion, 2)
        # print("gini_value : ", gini_value)
        return gini_value

    # 计算某一划分特征的基尼指数（基尼指数不等同于基尼值，多进行了加权平均）
    def computeOneFeatureGiniIndex(self, division_feature_id, sample_list=None):
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        gini_index = 0.0
        # 对样本列表根据划分特征进行拆分，得到字典（字典中的value为样本id列表）
        split_dict = self.splitSamplesList(division_feature_id, sample_list)
        # 计算拆分后各样本子列表的加权信息熵，并从信息增益中减去
        for division_feature_id in split_dict:
            sub_gini_value = self.computeGiniValue(split_dict[division_feature_id])  # 计算划分后的子样本集的信息熵
            sub_gini_value_weight = len(split_dict[division_feature_id]) / len(sample_list)  # 计算子样本集相应权重（子样本集的占比）
            gini_index += sub_gini_value * sub_gini_value_weight
        # print("gini_index : ", gini_index)
        return gini_index

    # 通过基尼系数的方式选出最合适的划分特征
    def selectDivisionFeatureByGiniIndex(self, available_division_feature_list=None, sample_list=None):
        # 注意根据基尼系数选择要选择最小的
        if sample_list is None:  # 没有给定sample_list，就认为是全部
            sample_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = 0
        min_gini_index = 1.0
        for division_feature_id in available_division_feature_list:
            temp_gini_index = self.computeOneFeatureGiniIndex(division_feature_id, sample_list)
            # print(temp_gini_index)
            if temp_gini_index < min_gini_index:
                chosen_division_feature_id = division_feature_id
                min_gini_index = temp_gini_index
        return chosen_division_feature_id

    # 将三种选取
    def getDivisionFeature(self, available_division_feature_list=None, sample_list=None, get_division_feature_method=user.entropy_gain):
        if get_division_feature_method == user.entropy_gain:
            return self.selectDivisionFeatureByEntropyGain(available_division_feature_list, sample_list)
        elif get_division_feature_method == user.gain_ratio:
            return self.selectDivisionFeatureByGainRatio(available_division_feature_list, sample_list)
        elif get_division_feature_method == user.gini_index:
            return self.selectDivisionFeatureByGiniIndex(available_division_feature_list, sample_list)

    # 判断给定的samples中
    def judgeSamplesHaveSameFeatures(self, features_list, samples_list):
        same_feature_flag = 1
        if len(samples_list) == 1:
            return 1
        for i in features_list:
            first_sample_feature = self.features[samples_list[0]][i]
            for j in samples_list[1:]:
                temp_feature = self.features[j][i]
                if temp_feature != first_sample_feature:
                    same_feature_flag = 0
                    return same_feature_flag
        return same_feature_flag

    # 获取列表形式的标签值
    def getLabelsPossibleValuesList(self):
        labels_possible_values_list = []
        for label_value in self.labels_possible_values:
            labels_possible_values_list.append(label_value)
        return labels_possible_values_list

    # 拆分数据集和训练集
    def seperateTrainSetAndTestSet(self, train_list=None):
        if train_list is None:
            train_list =
        pass

if __name__ == "__main__":
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.comprehensiveInitializeDataset()

    melon_dataset.selectDivisionFeatureByGiniIndex()
    pass

