# 1950083 自动化 刘智宇
from graphviz import Digraph

if __name__ == "__main__":
    dot = Digraph(comment='The Test Table')
    # 添加圆点A,A的标签是Dot A
    dot.node('A', 'Dot A')
    # 添加圆点 B, B的标签是Dot B
    dot.node('B', 'Dot B')
    # dot.view()
    # 添加圆点 C, C的标签是Dot C
    dot.node('C', 'Dot C')
    # dot.view()
    # 创建一堆边，即连接AB的两条边，连接AC的一条边。
    dot.edges(['AB', 'AC', 'AB'])
    # dot.view()
    # 在创建两圆点之间创建一条边
    dot.edge('B', 'C', 'test')
    # dot.view()
    # 获取DOT source源码的字符串形式
    print(dot.source)
    # // The Test Table
    # digraph {
    #   A [label="Dot A"]
    #   B [label="Dot B"]
    #   C [label="Dot C"]
    #   A -> B
    #   A -> C
    #   A -> B
    #   B -> C [label=test]
    # }
    # 保存source到文件，并提供Graphviz引擎
    # dot.render('test-output/test-table.gv', view=True)
