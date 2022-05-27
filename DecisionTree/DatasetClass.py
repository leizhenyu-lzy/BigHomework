# 1950083 自动化 刘智宇

# Thirdparty
import pandas as pd
import random

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
        self.features_continuity = []  # 连续特征标记（1表示连续，0表示离散）

        self.labels = []
        self.label_name = ""  # 标签索引名称

        self.features_possible_values = []  # 各个特征的全部可能的特征值，列表元素为字典，字典内为feature的值及对应个数
        self.labels_possible_values = {}  # 标签的可能值（字典,labels可能值和对应数量）

        self.train_sub_dataset_list = []
        self.test_sub_dataset_list = []

        # self.available_features_number_list = []  # not const

    # 高级初始化
    def comprehensiveInitializeDataset(self, continuity_list=None):
        self.readDataset(header=user.dataset_header, index_col=user.dataset_index_col)
        self.countFeaturesPossibleValues()  #
        self.countLabelsPossibleValues()  #
        self.setFeaturesContinuity(continuity_list)  # 确定是否为连续特征(pending)

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

    # 统计给定编号的数据的标签值(label)的种类及其个数
    def countLabelsPossibleValuesByList(self, samples_list=None):
        if samples_list is None:
            samples_list = [cnt for cnt in range(self.samples_amount)]
        if len(samples_list) == 0:  # 如果出现划分属性最大值有多个，会导致bigger部分没有值
            return {}
        # print(samples_list)
        labels_possible_values_dict = {self.labels[samples_list[0]]: 1}
        for sample_number in samples_list[1:]:  # 遍历给定样本列表的label
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

    # # 判断是否所有样本的label都相同
    # def judgeSamplesBelongSameCategory(self, samples_list=None):
    #     label_possible_values = self.countLabelsPossibleValuesByList(samples_list)
    #     if len(label_possible_values) == 1:
    #         # print("True ", list(label_possible_values.keys()))
    #         return True
    #     else:
    #         # print("False")
    #         return False

    # 判断特征的连续性
    def setFeaturesContinuity(self, continuity_list=None):
        if continuity_list is None or len(continuity_list) == 0:  # 没有传入连续值列表，默认全部不连续
            self.features_continuity = [0] * self.features_number
            return True
        elif len(continuity_list) == self.features_number:
            self.features_continuity = continuity_list
            return True
        else:  # 传入continuity_list但是长度对不上，说明list长度错误，打印错误信息
            print("[INFO] : The length of continuity_list is wrong.")
            return False

    # 通过给定特征拆分给定样本列表（需考虑是否为连续特征）
    def splitSamplesList(self, division_feature_id, samples_list=None, continuity_split_value=0):
        if samples_list is None:  # 没有给定samples_list，就统计所有值
            samples_list = [cnt for cnt in range(self.samples_amount)]
        split_dict = {}
        if self.features_continuity[division_feature_id]:  # 连续特征
            # print("splitSampleList : Continue")
            split_dict['bigger'] = []
            split_dict['smaller'] = []
            # 根据西瓜书：大于的归为一类，不大于的归为一类
            for sample_id in samples_list:
                if self.features[sample_id][division_feature_id] > continuity_split_value:
                    split_dict['bigger'].append(sample_id)
                else:
                    split_dict['smaller'].append(sample_id)
        else:  # 非连续特征
            # print("splitSampleList : Discrete")
            for division_feature_value in list(self.features_possible_values[division_feature_id].keys()):
                split_dict[division_feature_value] = []
            for sample_number in samples_list:
                split_dict[self.features[sample_number][division_feature_id]].append(sample_number)
            for division_feature_value in list(self.features_possible_values[division_feature_id].keys()):  # 避免空的列表返回
                if len(split_dict[division_feature_value]) == 0:
                    split_dict.pop(division_feature_value)  # 删除值为空列表的键值对
        return split_dict

    # 给定样本列表，计算信息熵（对于是否连续都适用，只统计label）
    def computeInformationEntropy(self, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]

        information_entropy = 0.0  # 信息熵
        label_values_dict = self.countLabelsPossibleValuesByList(samples_list)  # 统计标签值的字典

        for label_value in label_values_dict:
            label_proportion = (0.0 + label_values_dict[label_value])/len(samples_list)
            information_entropy -= user.pLog2p(label_proportion)
        # print(information_entropy)
        return information_entropy

    # 计算给定的划分特征、样本列表的信息增益（需考虑是否为连续特征）
    def computeOneFeatureEntropyGain(self, division_feature_id, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        # 先计算整体的信息熵
        overall_information_entropy = self.computeInformationEntropy(samples_list)
        entropy_gain = overall_information_entropy
        # print('division_feature_id: ', division_feature_id)
        if self.features_continuity[division_feature_id]:  # 连续特征
            feature_values_list = []
            for sample_id in samples_list:
                feature_values_list.append(self.features[sample_id][division_feature_id])
            feature_values_list.sort()
            # print("Sorted Value List : ", feature_values_list)
            best_entropy_gain_split_value = 0.0  # 最大信息增益对应的特征划分值
            max_entropy_gain = 0.0  # 最大的信息增益
            for value_id in [i+1 for i in range(len(feature_values_list)-1)]:  # 将所有候选划分点加入待测列表
                temp_entropy_gain = entropy_gain  # 每次循环更新原值
                continuity_split_value = (feature_values_list[value_id]+feature_values_list[value_id-1])/2
                split_dict = self.splitSamplesList(division_feature_id, samples_list, continuity_split_value)
                for division_feature_value in split_dict:
                    sub_information_entropy = self.computeInformationEntropy(split_dict[division_feature_value])  # 计算划分后的子样本集的信息熵
                    sub_information_entropy_weight = len(split_dict[division_feature_value]) / len(samples_list)  # 计算子样本集相应权重（子样本集的占比）
                    temp_entropy_gain -= sub_information_entropy * sub_information_entropy_weight
                # print("%2d: temp_entropy_gain: %.4f" % (value_id, temp_entropy_gain))
                # print("%2d: continuity_split_value: %.4f" % (value_id, continuity_split_value))
                if temp_entropy_gain > max_entropy_gain:  # 更新连续特征的最优划分值，更新最大的信息增益
                    best_entropy_gain_split_value = continuity_split_value
                    max_entropy_gain = temp_entropy_gain
            return max_entropy_gain, best_entropy_gain_split_value
        else:  # 非连续特征
            # 对样本列表根据划分特征进行拆分，得到字典（字典中的value为列表）
            split_dict = self.splitSamplesList(division_feature_id, samples_list)
            # 计算拆分后各样本子列表的加权信息熵，并从信息增益中减去
            for division_feature_value in split_dict:
                sub_information_entropy = self.computeInformationEntropy(split_dict[division_feature_value])  # 计算划分后的子样本集的信息熵
                sub_information_entropy_weight = len(split_dict[division_feature_value])/len(samples_list)  # 计算子样本集相应权重（子样本集的占比）
                entropy_gain -= sub_information_entropy * sub_information_entropy_weight
        return entropy_gain, None

    # 通过信息增益的方式选出最合适的划分特征
    def selectDivisionFeatureByEntropyGain(self, available_division_feature_list=None, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = None
        max_entropy_gain = 0.0
        best_split_value = None
        for division_feature_id in available_division_feature_list:
            temp_entropy_gain, split_value = self.computeOneFeatureEntropyGain(division_feature_id, samples_list)
            if temp_entropy_gain > max_entropy_gain:
                chosen_division_feature_id = division_feature_id
                max_entropy_gain = temp_entropy_gain
                best_split_value = split_value  # 对于连续特征特有的最佳划分特征
        print(chosen_division_feature_id, best_split_value)
        return chosen_division_feature_id, best_split_value

    # 计算某一特征的固有值intrinsic value
    def computeIntrinsicValue(self, division_feature_id, samples_list=None):
        intrinsic_value = 0.0
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        split_dict = self.splitSamplesList(division_feature_id, samples_list)
        for division_feature in split_dict:
            sub_samples_proportion = len(split_dict[division_feature])/len(samples_list)
            intrinsic_value -= user.pLog2p(sub_samples_proportion)
        # print("intrinsic_value : ", intrinsic_value)
        return intrinsic_value
    
    # 计算某一特征的信息增益率
    def computeOneFeatureGainRatio(self, division_feature_id, samples_list=None):
        feature_intrinsic_value = self.computeIntrinsicValue(division_feature_id, samples_list)  # 计算特征的固有值
        feature_information_gain = self.computeOneFeatureEntropyGain(division_feature_id, samples_list)[0]  # 计算特征的信息增益
        return feature_information_gain/feature_intrinsic_value  # 计算特征增益率

    # 通过信息增益率的方式选出最合适的划分特征
    def selectDivisionFeatureByGainRatio(self, available_division_feature_list=None, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = 0
        max_gain_ratio = 0.0
        for division_feature_id in available_division_feature_list:
            temp_gain_ratio = self.computeOneFeatureGainRatio(division_feature_id, samples_list)
            # print(temp_gain_ratio)
            if temp_gain_ratio > max_gain_ratio:
                chosen_division_feature_id = division_feature_id
                max_gain_ratio = temp_gain_ratio
        return chosen_division_feature_id, None

    # 计算基尼值
    def computeGiniValue(self, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部样本
            samples_list = [cnt for cnt in range(self.samples_amount)]

        gini_value = 1.0  # 使用1-[sigma(p)]^2的公式进行基尼值的度量，故从1.0开始减
        label_values_dict = self.countLabelsPossibleValuesByList(samples_list)  # 统计标签值的字典

        for label_value in label_values_dict:
            label_proportion = (0.0 + label_values_dict[label_value])/len(samples_list)
            gini_value -= pow(label_proportion, 2)
        # print("gini_value : ", gini_value)
        return gini_value

    # 计算某一划分特征的基尼指数（基尼指数不等同于基尼值，多进行了加权平均）
    def computeOneFeatureGiniIndex(self, division_feature_id, samples_list=None):
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        gini_index = 0.0
        # 对样本列表根据划分特征进行拆分，得到字典（字典中的value为样本id列表）
        split_dict = self.splitSamplesList(division_feature_id, samples_list)
        # 计算拆分后各样本子列表的加权信息熵，并从信息增益中减去
        for division_feature_id in split_dict:
            sub_gini_value = self.computeGiniValue(split_dict[division_feature_id])  # 计算划分后的子样本集的信息熵
            sub_gini_value_weight = len(split_dict[division_feature_id]) / len(samples_list)  # 计算子样本集相应权重（子样本集的占比）
            gini_index += sub_gini_value * sub_gini_value_weight
        # print("gini_index : ", gini_index)
        return gini_index

    # 通过基尼系数的方式选出最合适的划分特征
    def selectDivisionFeatureByGiniIndex(self, available_division_feature_list=None, samples_list=None):
        # 注意根据基尼系数选择要选择最小的
        if samples_list is None:  # 没有给定samples_list，就认为是全部
            samples_list = [cnt for cnt in range(self.samples_amount)]
        if available_division_feature_list is None:  # 没有给定available_division_feature_list，认为全部可用
            available_division_feature_list = [cnt for cnt in range(self.features_number)]
        chosen_division_feature_id = 0
        min_gini_index = 1.0
        for division_feature_id in available_division_feature_list:
            temp_gini_index = self.computeOneFeatureGiniIndex(division_feature_id, samples_list)
            # print(temp_gini_index)
            if temp_gini_index < min_gini_index:
                chosen_division_feature_id = division_feature_id
                min_gini_index = temp_gini_index
        return chosen_division_feature_id, None

    # 将三种选取最优划分特征的方法进行汇总（不关心连续或非连续）
    def getDivisionFeature(self, available_division_feature_list=None, samples_list=None, get_division_feature_method=user.entropy_gain):
        if get_division_feature_method == user.entropy_gain:
            return self.selectDivisionFeatureByEntropyGain(available_division_feature_list, samples_list)
        elif get_division_feature_method == user.gain_ratio:
            return self.selectDivisionFeatureByGainRatio(available_division_feature_list, samples_list)
        elif get_division_feature_method == user.gini_index:
            return self.selectDivisionFeatureByGiniIndex(available_division_feature_list, samples_list)

    # 判断给定的samples再给定的features十分完全相同（对是否连续都能处理）
    def judgeSamplesHaveSameFeatures(self, features_list, samples_list):
        same_feature_flag = 1
        if len(samples_list) == 1:
            return 1
        for i in features_list:  # 对特征循环
            first_sample_feature = self.features[samples_list[0]][i]  # 给定的第0个样本所有特征的值一定为新的
            for j in samples_list[1:]:  # 对样本循环
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

    # 拆分数据集和训练集(pending)
    def seperateTrainSetAndTestSet(self, train_dataset_numbers=0):
        if train_dataset_numbers > self.samples_amount:
            quit(-1)
        else:
            self.train_sub_dataset_list = []
            self.test_sub_dataset_list = [i for i in range(self.samples_amount)]  # 测试集挑剩下的
            while len(self.train_sub_dataset_list) < train_dataset_numbers:
                sample_idx = random.randrange(0, self.samples_amount)  # 使用randrange，上开下闭
                if sample_idx not in self.train_sub_dataset_list:
                    self.train_sub_dataset_list.append(sample_idx)
            for sample_idx in self.train_sub_dataset_list:
                self.test_sub_dataset_list.remove(sample_idx)
            print("train_sub_dataset_list   : ", self.train_sub_dataset_list)
            print("test_sub_dataset_list    : ", self.test_sub_dataset_list)


if __name__ == "__main__":
    # melon_dataset = Dataset(dataset_path=user.DatasetPath, dataset_name=user.DatasetName)
    # melon_dataset.comprehensiveInitializeDataset(continuity_list=[1, 1])
    # melon_dataset.printDatasetInfo()
    #
    # # melon_dataset.computeOneFeatureEntropyGain(division_feature_id=0)
    # melon_dataset.selectDivisionFeatureByEntropyGain()
    pass
