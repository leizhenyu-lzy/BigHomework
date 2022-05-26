# 1950083 自动化 刘智宇
import UserToolFunction

"""
节点类设计
类变量
1. 类型（是否为叶节点）
2. 子节点列表
3. 父节点
4. 当前节点划分特征
5. 子节点对应的划分特征

成员函数：
1. 打印
2. 向下遍历
"""
class DecisionTreeNode:
    global_node_id = 0  # 类共享变量（在有新treenode创建时会自增）

    # 定义类变量
    def __init__(self):
        self.node_id = DecisionTreeNode.global_node_id  # 节点编号（单个数值）
        DecisionTreeNode.global_node_id += 1  # 共享变量自增

        self.parentnode = None  # 父节点（单个节点）
        self.parentnode_id = None  # 父节点编号（单个数值）
        self.childnodes_list = []  # 子节点列表（节点列表）
        self.childnodes_id_list = []  # 子节点编号列表（数值列表）

        self.division_feature_id = None  # 当前节点用于划分子节点的划分特征编号（单个数值）
        self.childnode_division_feature_values = []  # 子节点对应划分特征的特征值（特征值列表）
        # （对于连续值，二分法，存放相同的值，默认左节点为'smaller'，右节点为'bigger'）
        self.continuous_feature_split_value = None  # 若当前的划分特征为连续值，用于记录最佳划分特征值

        self.include_samples = []  # 当前节点包含的样本编号（数值列表）
        self.available_features_id = []  # 可用的划分特征列表，从父节点的改成员变量删去父节点划分特征所得（数值列表）
        self.final_label = None  # 对于叶节点，给出归到这里的样本的最终决策结果（标注的实际值）
        # print(self.global_node_id)

    # 输出节点信息（节点都用编号表示）
    def getNodeInfo(self):
        UserToolFunction.printSeparateBar()
        print("node_id              : ", self.node_id)
        print("parentnode           : ", self.parentnode_id)
        print("childnodes_list      : ", self.childnodes_id_list)
        print("div_feature_id       : ", self.division_feature_id)
        print("continue_split_value : ", self.continuous_feature_split_value)
        print("include_samples      : ", self.include_samples)
        print("final_label          : ", self.final_label)
        UserToolFunction.printSeparateBar()

    # 给当前节点添加子节点，同时更新当前节点及后续节点中的指向信息
    def attachChildNode(self, childnode, childnode_feature_value):
        # 父节点指向子节点
        self.childnodes_list.append(childnode)
        self.childnodes_id_list.append(childnode.node_id)
        # 子节点指向父节点
        childnode.parentnode = self
        childnode.parentnode_id = self.node_id
        # 父子节点其余信息调整
        self.childnode_division_feature_values.append(childnode_feature_value)

    # 更新当前节点的划分特征
    def updateDivisionProperty(self, div_feature):
        self.division_feature_id = div_feature

    # 深度优先打印以当前节点为根节点的决策树
    def printDecisionTreeDepthFirst(self):
        self.getNodeInfo()
        if len(self.childnodes_list):  # 子节点列表非空
            for childnode in self.childnodes_list:
                childnode.printDecisionTreeDepthFirst()

    # 广度优先打印以当前节点为根节点的决策树
    def printDecisionTreeBreadthFirst(self):
        node_queue = [self]
        queue_pointer = 0
        while queue_pointer < len(node_queue):
            node_queue.extend(node_queue[queue_pointer].childnodes_list)
            queue_pointer += 1
        for node in node_queue:
            node.getNodeInfo()

    def setIncludeSample(self, include_sample_list):
        self.include_samples = include_sample_list

    def setAvailableFeaturesId(self, available_feature_id_list):
        self.available_features_id = available_feature_id_list

    def saveDecisionTreeToFile(self, filename="DecisionTree.txt"):
        pass

    def readDecisionTreeFromFile(self, filename="DecisionTree.txt"):
        pass

    # 判断是不是叶子节点
    def isLeaf(self):
        if not len(self.childnodes_id_list):  # 子节点非空
            return True
        else:  # 没有子节点
            return False

    # 获取样本通过决策树得到的结果（递归方式）(pending:对于连续值的处理)（传入样本的全部特征的值）
    def getSampleLabelByDecisionTree(self, sample_features_list, features_continuity_list):
        if not self.isLeaf():  # 当前节点不为叶节点，需要进行细分
            if features_continuity_list[self.division_feature_id] == 1:  # 当前节点的划分特征为连续特征
                if sample_features_list[self.division_feature_id] <= self.continuous_feature_split_value:  # 特征值不大于划分值
                    use_childnode = self.childnodes_list[0]  # 选择使用左节点（规定左节点为smaller）
                else:
                    use_childnode = self.childnodes_list[1]  # 选择使用右节点（规定右节点为bigger）
                final_label = use_childnode.getSampleLabelByDecisionTree(sample_features_list, features_continuity_list)
            else:  # 当前节点的划分特征为离散特征
                sample_division_feature_value = sample_features_list[self.division_feature_id]  # 获取该样本对应当前节点的划分特征的值
                # 获取样本划分特征的值所对应的子节点划分特征的值的列表中的序号，即后续递归使用的子节点在当前节点的子节点列表中的序号
                division_feature_value_idx = self.childnode_division_feature_values.index(sample_division_feature_value)
                use_childnode = self.childnodes_list[division_feature_value_idx]
                final_label = use_childnode.getSampleLabelByDecisionTree(sample_features_list, features_continuity_list)
                # use_sub_node = self.childnodes_list[]
            return final_label
        else:  # 当前节点为叶节点，当前节点的final_label即为样本的分类最终结果
            return self.final_label

    # 获取当前决策树混淆矩阵（字典形式）（对于是否连续不关心）
    def getConfusionMatrixDict(self, dataset):
        # 先创建字典的字典，外层字典用于表示实际label，内层字典用于表示通过决策树得到的label
        confusion_matrix = {}
        for outer_label_value in dataset.labels_possible_values:
            confusion_matrix[outer_label_value] = {}
            for inner_label_value in dataset.labels_possible_values:
                confusion_matrix[outer_label_value][inner_label_value] = 0
        # 对每一个样本进行划分，按照实际label和决策树给出的label放入相应的混淆矩阵中
        for sample_id in range(dataset.samples_amount):
            outer_label_value = dataset.labels[sample_id]  # 实际label
            # 决策树给出的label（传入一个样本的全部特征）
            inner_label_value = self.getSampleLabelByDecisionTree(dataset.features[sample_id], dataset.features_continuity)
            confusion_matrix[outer_label_value][inner_label_value] += 1
        return confusion_matrix


if __name__ == "__main__":
    pass

