package com.lzy.simworld;

import com.google.gson.Gson;

import java.util.Objects;

public class Coordinate
{
    public int row = 0;
    public int col = 0;

    public Coordinate(int outerRow, int outerCol)
    {
        this.row = outerRow;
        this.col = outerCol;
    }

    public Coordinate(Coordinate outerCoordinate)
    {
        this.row = outerCoordinate.row;
        this.col = outerCoordinate.col;
    }

    public void modifyCoordinate(int outerRow, int outerCol)
    {
        this.row = outerRow;
        this.col = outerCol;
    }

    // 用括号坐标形式输出坐标点
    public String printCoordinate()
    {
        String tempString = "("+ this.row +","+ this.col +")";
        System.out.print(tempString);
        return tempString;
    }

    // 获得点的括号坐标形式字符串
    public String getCoordinateInfoString()
    {
        String tempString = "("+ this.row +","+ this.col +")";
        // System.out.println(tempString);
        return tempString;
    }

    // 通过Gson获取Coordinate的Json字符串
    public String getCoordinateJsonString()
    {
        Gson gson = new Gson();
        String jsonString = gson.toJson(this);
        // System.out.println(jsonString);
        return jsonString;
    }


    // 重写equals方法以便使用ArrayList的indexOf方法
    @Override
    public boolean equals(Object obj)
    {
        if (this == obj)
            return true;
        if (obj == null || getClass() != obj.getClass())
            return false;
        Coordinate that = (Coordinate) obj;
        return this.row == that.row && this.col == that.col;
    }

    public Coordinate getMagnifiedCoord(final int magnifyRate)
    {
        return new Coordinate(this.row*magnifyRate,this.col*magnifyRate);
    }


    public static void main(String[] args)
    {
        // 测试Gson
        Coordinate newCoord = new Coordinate(1,2);
        System.out.println(newCoord.getCoordinateJsonString());
        // {"row":1,"col":2}
    }
}
