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

    # 得到最优划分特征编号，以及最优划分值（连续特征独有）
    best_division_feature_id, best_division_feature_value = dataset.getDivisionFeature(current_node.available_features_id,
                                                                                       current_node.include_samples, get_division_feature_method)
    # best_division_feature_name = dataset.features_name[best_division_feature_id]
    # print(best_division_feature_name)
    sub_node_available_features = current_node.available_features_id.copy()
    if not dataset.features_continuity[best_division_feature_id]:  # 对于非连续特征，作为划分属性后，其子节点不应再使用
        sub_node_available_features.remove(best_division_feature_id)  # 选出了最佳划分特征，后面的决策应不受该特征影响
    current_node.division_feature_id = best_division_feature_id
    # print(current_node.division_feature_id)

    # 根据最优划分特征，将样本集进行拆分
    split_dict = dataset.splitSamplesList(division_feature_id=best_division_feature_id, samples_list=current_node.include_samples,
                                          continuity_split_value=best_division_feature_value)
    # print(split_dict)

    if dataset.features_continuity[best_division_feature_id]:  # 对于连续特征，只有bigger、smaller两个节点
        current_node.continuous_feature_split_value = best_division_feature_value  # 设置连续特征的最佳划分值
        # 设置smaller节点
        smaller_sub_node = DecisionTreeNode()  # 添加smaller子节点
        current_node.attachChildNode(smaller_sub_node, 'smaller')
        smaller_sub_node.available_features_id = sub_node_available_features
        smaller_sub_node.setIncludeSample(split_dict['smaller'])
        generateDecisionTree(dataset, smaller_sub_node, get_division_feature_method)
        # 设置bigger节点
        bigger_sub_node = DecisionTreeNode()  # 添加bigger子节点
        current_node.attachChildNode(bigger_sub_node, 'bigger')
        bigger_sub_node.available_features_id = sub_node_available_features
        bigger_sub_node.setIncludeSample(split_dict['bigger'])
        generateDecisionTree(dataset, bigger_sub_node, get_division_feature_method)

    else:  # 对于离散特征，需要将划分特征的每一个特征值都建立树节点
        for feature_value in dataset.features_possible_values[best_division_feature_id]:  # 需要给该特征的每一个可能值都建立树节点，即使没有可能值
            sub_node = DecisionTreeNode()  # 生成新的子节点
            current_node.attachChildNode(sub_node, feature_value)  # 将子节点与当前节点进行连接
            sub_node.available_features_id = sub_node_available_features
            if split_dict.get(feature_value, 0):  # 如果拆分出的字典键值中有feature_value
                sub_node.setIncludeSample(split_dict[feature_value])
                generateDecisionTree(dataset, sub_node, get_division_feature_method)  # 递归调用
            else:  # 如果拆分出的字典键值中没有feature_value，即没有出现过该种情况
                sub_node.final_label = most_possible_label_value  # 作为叶子节点，final_label设为最可能的值
    return


if __name__ == "__main__":
    # 创建数据集
    dataset = Dataset(dataset_path=user.DatasetPath, dataset_name=user.DatasetName)
    dataset.comprehensiveInitializeDataset(continuity_list=user.DatasetContinuityList)
    # melon_dataset.printDatasetInfo()

    # 创建决策树
    user.printSeparateBar()
    print("Start generating a decision tree.")
    root_node = DecisionTreeNode()  # 创建根节点
    root_node.setIncludeSample([cnt for cnt in range(dataset.samples_amount)])  # 根节点包含所有样本
    root_node.setAvailableFeaturesId([cnt for cnt in range(dataset.features_number)])  # 根节点拥有所有划分特征的选择权
    # 生成决策树
    generateDecisionTree(dataset=dataset, current_node=root_node, get_division_feature_method=user.get_division_feature_method)
    print("Finish generating a decision tree")
    user.printSeparateBar()

    # # 打印决策树信息广度优先方式方式，方便横向查看
    root_node.printDecisionTreeBreadthFirst()
    # # 打印决策树信息深度优先方式方式，方便横向查看
    # # root_node.printDecisionTreeDepthFirst()

    confusion_matrix_dict = root_node.getConfusionMatrixDict(dataset)  # 获得字典形式的混淆矩阵
    confusion_matrix_nparray = viz.dictConfusionMatrixToNpArray(confusion_matrix_dict)  # 字典形式的混淆矩阵转为np.array形式
    viz.drawConfusionMatrix(confusion_matrix_nparray, dataset.getLabelsPossibleValuesList(), fig_name=user.viz_decision_tree_name)

    # 使用Graphviz库图形化展示决策树，并将图片进行保存
    tree_graph = viz.drawDecisionTree(root_node, dataset.features_name, dataset.features_continuity, user.viz_decision_tree_name)
    tree_graph.view(cleanup=True)  # 清除除了png图片以外的其他多余文件



