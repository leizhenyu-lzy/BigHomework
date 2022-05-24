# 1950083 自动化 刘智宇

# Thirdparty

# User
import UserToolFunction as user
from DecisionTreeNodeClass import DecisionTreeNode
from DatasetClass import Dataset
import DecisionTreeVisiualize as viz


def generateDecisionTree(dataset, current_node, get_division_feature_method):
    labels_possible_values_dict = dataset.countLabelsPossibleValuesByList(current_node.include_samples)
    most_possible_label_value = user.getMaxValueIndex(labels_possible_values_dict)  # 得到当前序列最大的值，对列表和数组都适用

    # 判断样本是否全属于同一类别，如果是则将该treenode标记为该类的叶节点
    if len(labels_possible_values_dict) == 1:
        current_node.final_label = list(labels_possible_values_dict.keys())[0]  # 将此节点作为叶子节点，并最终标记其输出label
        return

    # 判断特征集是否为空或所有样本在特征集上取值相同，则将该节点标记为叶节点，其label为最可能的
    if (not len(current_node.available_features_id)) or \
            dataset.judgeSamplesHaveSameFeatures(current_node.available_features_id, current_node.include_samples):
        current_node.final_label = most_possible_label_value
        # print(most_possible_label_value)
        return

    # 得到最优划分特征
    best_division_feature_id = dataset.getDivisionFeature(current_node.available_features_id,
                                                          current_node.include_samples, get_division_feature_method)
    # best_division_feature_name = dataset.features_name[best_division_feature_id]
    # print(best_division_feature_name)
    sub_node_available_features = current_node.available_features_id.copy()
    sub_node_available_features.remove(best_division_feature_id)  # 选出了最佳划分特征，后面的决策应不受该特征影响
    current_node.division_feature_id = best_division_feature_id
    # print(current_node.division_feature_id)

    # 根据最优划分特征，将样本集进行拆分
    split_dict = dataset.splitSamplesList(division_feature_id=best_division_feature_id, sample_list=current_node.include_samples)
    # print(split_dict)
    for feature_value in dataset.features_possible_values[best_division_feature_id]:  # 需要给该特征的每一个可能值都建立树节点，即使没有可能值
        sub_node = DecisionTreeNode()  # 生成新的子节点
        current_node.attachChildNode(sub_node, feature_value)  # 将子节点与当前节点进行连接
        sub_node.available_features_id = sub_node_available_features
        if split_dict.get(feature_value, 0):  # 如果拆分出的字典键值中有feature_value
            sub_node.setIncludeSample(split_dict[feature_value])
            generateDecisionTree(dataset, sub_node, get_division_feature_method)  # 递归调用
        else:  # 如果拆分出的字典键值中没有feature_value
            sub_node.final_label = most_possible_label_value
    return


if __name__ == "__main__":
    # 数据集操作
    melon_dataset = Dataset(dataset_path=user.MelonDatasetPath, dataset_name="MelonDataset")
    melon_dataset.comprehensiveInitializeDataset()

    # 生成决策树
    user.printSeparateBar()
    print("Start generating a decision tree.")
    root_node = DecisionTreeNode()  # 创建根节点
    root_node.setIncludeSample([cnt for cnt in range(melon_dataset.samples_amount)])  # 树节点包含所有样本
    root_node.setAvailableFeaturesId([cnt for cnt in range(melon_dataset.features_number)])
    # root_node.setIncludeSample([cnt for cnt in range(10, 13)])  # 树节点包含的部分样本
    generateDecisionTree(dataset=melon_dataset, current_node=root_node,
                         get_division_feature_method=user.get_division_feature_method)
    print("Finish generating a decision tree")
    user.printSeparateBar()

    # 打印决策树信息广度优先方式方式，方便横向查看
    root_node.printDecisionTreeBreadthFirst()
    # 打印决策树信息深度优先方式方式，方便横向查看
    # root_node.printDecisionTreeDepthFirst()

    decicion_tree_name = melon_dataset.dataset_name+user.get_division_feature_methods_name[user.get_division_feature_method]
    # print(decicion_tree_name)

    # 使用Graphviz库图形化展示决策树，并将图片进行保存
    tree_graph = viz.drawDecisionTree(root_node, melon_dataset.features_name, decicion_tree_name)
    tree_graph.view(cleanup=True)  # 清除除了png图片以外的其他多余文件

    # 随便给当前决策树一个样本，判断其label值
    # sample_final_label = root_node.getSampleLabelByDecisionTree(melon_dataset.features[14])
    # print(sample_final_label)

    confusion_matrix = root_node.getConfusionMatrixDict(melon_dataset)
    confusion_matrix_nparray = viz.dictConfusionMatrixToList(confusion_matrix)
    viz.drawConfusionMatrix(confusion_matrix_nparray, melon_dataset.getLabelsPossibleValuesList(),
                            fig_name=decicion_tree_name+'ConfusionMatrix')

