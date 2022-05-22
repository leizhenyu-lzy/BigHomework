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
        self.childnode_division_feature_value = []  # 子节点对应划分特征的特征值（字符串列表）
        self.include_samples = []  # 当前节点包含的样本编号（数值列表）
        self.available_features_id = []  # 可用的划分特征列表，从父节点的改成员变量删去父节点划分特征所得（数值列表）
        self.final_label = None  # 对于叶节点，给出归到这里的样本的最终决策结果（标注的实际值）
        # print(self.global_node_id)

    # 输出节点信息（节点都用编号表示）
    def getNodeInfo(self):
        UserToolFunction.printSeparateBar()
        print("node_id          : ", self.node_id)
        print("parentnode       : ", self.parentnode_id)
        print("childnodes_list  : ", self.childnodes_id_list)
        print("div_feature_id   : ", self.division_feature_id)
        print("include_samples  : ", self.include_samples)
        print("final_label      : ", self.final_label)
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
        self.childnode_division_feature_value.append(childnode_feature_value)

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
        if len(self.childnodes_id_list):  # 子节点非空
            return True
        else:  # 没有子节点
            return False


if __name__ == "__main__":
    pass

