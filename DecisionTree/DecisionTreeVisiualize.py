# 1950083 自动化 刘智宇

# Thirdparty
import graphviz

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
            print('leaf')
            setGraphNodeAttribute(graph, 'leaf')  # 设置节点属性
            graph.node(name=str(childnode.node_id), label=childnode.final_label)
            print(childnode.final_label)
        else:  # 不是叶节点
            print('middle')
            setGraphNodeAttribute(graph, 'middle')  # 设置节点属性
            graph.node(name=str(childnode.node_id),
                       label=features_name_list[childnode.division_feature_id] + '=?')
        graph.view()
        # 画连线
        graph.edge(str(current_node.node_id), str(childnode.node_id),
                   label=current_node.childnode_division_feature_value[childnode_idx])
        print(current_node.childnode_division_feature_value[childnode_idx])

# 画整颗树
def drawDecisionTree(root_node, feature_name_list):
    tree_graph = graphviz.Graph(name="DecisionTree", filename="DecisionTree",
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
        pass
        # node.getNodeInfo()
    return tree_graph


if __name__ == "__main__":
    pass
