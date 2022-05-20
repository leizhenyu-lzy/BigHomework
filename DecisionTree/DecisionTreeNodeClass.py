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
    global_node_number = 0

    # 定义类变量
    def __init__(self):
        self.node_num = self.global_node_number  # 节点编号
        self.parentnode = None  # 父节点
        self.parentnode_number = None  # 父节点编号
        self.childnodes_list = []  # 子节点列表
        self.childnodes_number_list = []  # 子节点编号列表
        self.division_feature = None
        self.childnode_division_feature_value = []  # 子节点对应划分特征的特征值
        self.include_samples = []  # 当前节点包含的样本编号
        self.global_node_number += 1
        self.final_label = ""  # 对于叶节点，给出归到这里的样本的最终决策结果
        print(self.global_node_number)

    # 输出节点信息（节点都用编号表示）
    def getNodeInfo(self):
        UserToolFunction.printSeparateBar()
        print("node_number      : ", self.node_number)
        print("parentnode       : ", self.parentnode_number)
        print("childnodes_list       : ", self.childnodes_number_list)
        print("div_feature      : ", self.division_feature)
        print("include_samples  : ", self.division_feature)
        print("div_property : ", self.division_feature)
        print("div_property : ", self.division_feature)
        UserToolFunction.printSeparateBar()

    # 给当前节点添加子节点，同时更新当前节点及后续节点中的指向信息
    def addChildNode(self, childnode, childnode_feature_value):
        self.childnodes_list.append(childnode)
        self.childnodes_number_list.append(childnode.node_number)
        self.childnode_division_feature_value.append(childnode_feature_value)
        childnode.parentnode = self.node_number

    # 更新当前节点的划分特征
    def updateDivisionProperty(self, div_feature):
        self.division_feature = div_feature

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
            queue_pointer = queue_pointer + 1
        for node in node_queue:
            node.getNodeInfo()

    def setIncludeSample(self, include_sample_lists):
        self.include_samples = include_sample_lists

    def saveDecisionTreeToFile(self, filename="DecisionTree.txt"):
        pass

    def readDecisionTreeFromFile(self, filename="DecisionTree.txt"):
        pass

    # 判断是不是叶子节点
    def judgeLeafNode(self):
        if len(self.childnodes_number_list):  # 子节点非空
            return True
        else:  # 没有子节点
            return False


if __name__ == "__main__":
    pass

