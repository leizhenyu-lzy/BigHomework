# 1950083 自动化 刘智宇
import graphviz
from graphviz import Digraph
from DecisionTreeVisualize import *

if __name__ == "__main__":
    dot = Digraph(comment='The Test Table', format='png')
    dot.attr('node', shape=leaf_shape, style=node_style, color=leaf_color, fontname="NSimSun", fontsize='20')
    dot.attr('edge', fontname="NSimSun", fontsize='15')
    # 添加圆点A,A的标签是Dot A
    dot.node(name='1', label='傻逼')
    # 添加圆点 B, B的标签是Dot B
    dot.node('23', 'Dot B')
    # dot.view()
    # 添加圆点 C, C的标签是Dot C
    dot.attr('node', shape=middle_shape, style=node_style, color=middle_color, fontname="Microsoft Yahei")
    dot.node('C', '傻逼')
    # dot.view()
    # 创建一堆边，即连接AB的两条边，连接AC的一条边。
    dot.edges(['123', '1C', '123'])
    # dot.view()
    # 在创建两圆点之间创建一条边
    dot.edge('23', 'C', '草泥马')
    # dot.view()
    # 获取DOT source源码的字符串形式
    print(dot.source)
    # 保存source到文件，并提供Graphviz引擎
    dot.view(cleanup=True)
    # dot.render('test-output/test-table', view=False, cleanup=True)


