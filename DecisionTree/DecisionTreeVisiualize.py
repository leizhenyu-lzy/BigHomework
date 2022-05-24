# 1950083 自动化 刘智宇

# Thirdparty
import graphviz
import matplotlib.pyplot as plt
import numpy as np
import os.path

# User
from DecisionTreeNodeClass import DecisionTreeNode
from DatasetClass import Dataset


# 定义常量
leaf_shape = 'ellipse'
leaf_color = 'lightblue'

middle_shape = 'box'
middle_color = 'lightgreen'

node_fontsize = '20'
node_fontname = 'NSimSun'
node_style = 'filled'

edge_fontsize = '15'
edge_fontname = 'NSimSun'

# 设置
def setGraphEdgeAttribute(graph):
    graph.attr('edge', fontname=edge_fontname, fontsize=edge_fontsize)

def setGraphNodeAttribute(graph, node_type):
    if node_type == 'leaf':  # 是叶节点
        graph.attr('node', shape=leaf_shape, style=node_style, color=leaf_color,
                   fontname=node_fontname, fontsize=node_fontsize)
    elif node_type == 'middle':  # 不是叶节点
        graph.attr('node', shape=middle_shape, style=node_style, color=middle_color,
                   fontname=node_fontname, fontsize=node_fontsize)


# 每个节点指负责画子节点和连线
def drawOneTreeNode(graph, current_node, features_name_list):
    for childnode_idx in range(len(current_node.childnodes_id_list)):  # childnode_idx是指childnode在
        childnode = current_node.childnodes_list[childnode_idx]
        # 画子节点
        if childnode.isLeaf():  # 叶节点是
            # print('leaf')
            setGraphNodeAttribute(graph, 'leaf')  # 设置节点属性
            graph.node(name=str(childnode.node_id), label=childnode.final_label)
            # print(childnode.final_label)
        else:  # 不是叶节点
            # print('middle')
            setGraphNodeAttribute(graph, 'middle')  # 设置节点属性
            graph.node(name=str(childnode.node_id),
                       label=features_name_list[childnode.division_feature_id] + '=?')
        # graph.view()
        # 画连线
        graph.edge(str(current_node.node_id), str(childnode.node_id),
                   label=current_node.childnode_division_feature_values[childnode_idx])
        # print(current_node.childnode_division_feature_values[childnode_idx])

# 画整颗树
def drawDecisionTree(root_node, feature_name_list, filename=""):
    tree_graph = graphviz.Graph(name="DecisionTree", filename=filename,
                                directory="SavedDecisionTree", format='png')
    setGraphNodeAttribute(tree_graph, 'middle')
    setGraphEdgeAttribute(tree_graph)
    tree_graph.node(name=str(root_node.node_id), label=feature_name_list[root_node.division_feature_id] + '=?')
    # tree_graph.view(cleanup=True)
    node_queue = [root_node]  # 将根节点添加进节点列表
    queue_pointer = 0
    while queue_pointer < len(node_queue):
        node_queue.extend(node_queue[queue_pointer].childnodes_list)
        queue_pointer += 1
    for node in node_queue:
        drawOneTreeNode(tree_graph, node, feature_name_list)
        # node.getNodeInfo()
    return tree_graph


# 在终端中展示混淆矩阵
def dictConfusionMatrixToList(confusion_mat_dict, show_flag=False):
    confusion_mat_list = []
    for i in confusion_mat_dict:
        temp_list = list(confusion_mat_dict[i].values())
        confusion_mat_list.append(temp_list)
        if show_flag is True:
            print(temp_list)
    return np.array(confusion_mat_list)


# 用matplotlib绘制混淆矩阵并保存
def drawConfusionMatrix(confusion_matrix_nparray, label_values, fig_name):
    plt.figure()
    plt.title('ConfusionMatrix')
    # plt.axis('off')
    plt.ylabel("Actual Label")  # 纵轴
    plt.xlabel("Decision Tree Generate Label")  # 横轴

    plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签

    # 在坐标轴上显示feature_values
    tick_marks = np.arange(len(label_values))
    plt.xticks(tick_marks, label_values)  # , rotation=45)
    plt.yticks(tick_marks, label_values)

    # 显示混淆矩阵
    plt.imshow(confusion_matrix_nparray, cmap=plt.cm.Blues)
    for i in range(len(label_values)):
        for j in range(len(label_values)):
            # 这里i，j需要反过来，否则会转置关系
            plt.text(j, i, confusion_matrix_nparray[i][j], fontdict={'fontsize': 20}, ha='center', va='center')
    save_fig_path = os.path.join('SavedDecisionTree', fig_name+'.png')
    plt.savefig(save_fig_path)
    plt.show(block=False)  # 一定要先保存图片再进行显示，否则保存的图片为空白
    plt.pause(3)  # 显示3秒自动关闭窗口，再plt.show中需要添加block=False
    plt.close()


if __name__ == "__main__":
    conf_np_array = dictConfusionMatrixToList({1:{1:41,2:12,3:13},2:{3:23,4:24,5:25},3:{5:35,6:36,7:37}})
    # print(conf_np_array)
    drawConfusionMatrix(conf_np_array, ['da', 'sha', 'bi'], "haha")



