package com.lzy.simworld;

import java.util.Arrays;

public class ToolFunctions
{
    // 计算曼哈顿距离
    public static int calculateManhattanDistance(final int row1, final int col1, final int row2, final int col2)
    {
        int manhattanDistance = Math.abs(row1-row2) + Math.abs(col1-col2);
        return manhattanDistance;
    }
    // 计算欧式距离
    public static int calculateEuclideanDistance(final int row1, final int col1, final int row2, final int col2)
    {
        int euclideanDistance;
        int deltaRow = Math.abs(row1-row2);
        int deltaCol = Math.abs(col1-col2);
        euclideanDistance = (int)(Math.sqrt(Math.pow(deltaRow,2)+Math.pow(deltaCol,2)));
        return euclideanDistance;
    }
    // 可选择进行欧式距离计算或是曼哈顿距离计算
    public static int calculateDistance(final int methodChoice, final Coordinate coord1, final Coordinate coord2)
    {
        int distance = 0;
        if(methodChoice==Consts.USE_MANHATTAN_DISTANCE)
        {
            distance = calculateManhattanDistance(coord1.row, coord1.col, coord2.row, coord2.col);
        }
        else if(methodChoice == Consts.USE_EUCLIDEAN_DISTANCE)
        {
            distance = calculateEuclideanDistance(coord1.row, coord1.col, coord2.row, coord2.col);
        }
        return distance;
    }

    // 获取随机颜色
    public static int[] getRandomRGBColor()
    {
        int[] rgbColor = new int[3];
        for(int i=0;i<3;i++)
        {
            rgbColor[i] = (int)(Math.random()*255);
        }
        // System.out.println(Arrays.toString(rgbColor));
        return rgbColor;
    }




    public static void main(String[] args)
    {
        // 测试距离计算
        // System.out.println(ToolFunctions.calculateManhattanDistance(1,2,5,5));
        // System.out.println(ToolFunctions.calculateEuclideanDistance(1,2,5,5));

        // 测试随机颜色生成
        // for(int i = 5;i<10;i++)
        // {
        //     getRandomRGBColor();
        // }
    }
}
