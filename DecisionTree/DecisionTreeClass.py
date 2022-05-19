# 1950083 自动化 刘智宇
import UserToolFunction

"""
节点类设计
类变量
1. 类型（是否为叶节点）
2. 子节点列表
3. 父节点
4. 当前节点划分属性类
5. 子节点对应的划分属性属性

成员函数：
1. 打印
2. 向下遍历
"""
class DecisionTreeNode:
    # 定义类变量
    def __init__(self, node_num):
        self.node_number = node_num  # 节点编号
        self.parentnode = None  # 父节点编号
        self.parentnode_number = None  # 父节点编号
        self.childnodes = []  # 子节点列表
        self.childnodes_number = []  # 子节点编号
        self.childnode_property_value = []  # 子节点对应划分属性的属性值
        self.division_property = ""
        self.include_samples = []  # 当前节点包含的样本

    # 输出节点信息（节点都用编号表示）
    def getNodeInfo(self):
        UserToolFunction.printSeparateBar()
        print("node_number  : ", self.node_number)
        print("parentnode   : ", self.parentnode_number)
        print("childnodes   : ", self.childnodes_number)
        print("div_property : ", self.division_property)
        UserToolFunction.printSeparateBar()

    # 给当前节点添加子节点，同时更新当前节点及后续节点中的指向信息
    def addChildNode(self, childnode, childnode_property_value):
        self.childnodes.append(childnode)
        self.childnodes_number.append(childnode.node_number)
        self.childnode_property_value.append(childnode_property_value)
        childnode.parentnode = self.node_number

    # 更新当前节点的划分属性
    def updateDivisionProperty(self, div_property):
        self.division_property = div_property

    # 深度优先打印以当前节点为根节点的决策树
    def printDecisionTreeDepthFirst(self):
        self.getNodeInfo()
        if len(self.childnodes):  # 子节点列表非空
            for childnode in self.childnodes:
                childnode.printDecisionTreeDepthFirst()

    # 广度优先打印以当前节点为根节点的决策树
    def printDecisionTreeBreadthFirst(self):
        node_queue = [self]
        queue_pointer = 0
        while queue_pointer < len(node_queue):
            node_queue.extend(node_queue[queue_pointer].childnodes)
            queue_pointer = queue_pointer + 1
        for node in node_queue:
            node.getNodeInfo()

    def setIncludeSample(self, include_sample_lists):



    def saveDecisionTreeToFile(self, filename="DecisionTree.txt"):
        pass

    def readDecisionTreeFromFile(self, filename="DecisionTree.txt"):
        pass


if __name__ == "__main__":
    pass

