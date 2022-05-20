# 1950083 自动化 刘智宇

# Thirdparty
import math

# User
# import DatasetFunction

"""
函数名称：pLog2p
函数作用：计算p*log2(p),便于计算信息熵以及信息增益
函数输入：概率p
函数返回：p*log2(p)的值
"""
def pLog2p(possibility):
    possibility = float(possibility)
    return possibility * math.log(possibility, 2)


if __name__ == "__main__":
    pass

