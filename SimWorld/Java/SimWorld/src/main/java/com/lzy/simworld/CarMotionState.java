package com.lzy.simworld;

import com.google.gson.Gson;

public class CarMotionState
{
    private Coordinate location = new Coordinate(0,0);  // 汽车位置
    private int orientation = 0;  // 汽车朝向

    // 初始化时提供起始Coord
    public CarMotionState(Coordinate originCoord)
    {
        this.location.row = originCoord.row;
        this.location.col = originCoord.col;
        this.orientation = 0;  // 默认角度是水平向右，即x-y坐标系的x方向
    }

    // 无参构造
    public CarMotionState() {    }


    // 更新汽车坐标
    public void updateLocation(Coordinate newCoord)
    {
        this.location.row = newCoord.row;
        this.location.col = newCoord.col;
    }

    // 更新汽车朝向
    public void updateRotation(int newOrientation)
    {
        this.orientation = newOrientation;
    }

    public Coordinate getLocation()
    {
        return this.location;
    }
    public int getOrientation()
    {
        return this.orientation;
    }

    public String getCarMotionStateJsonString()
    {
        Gson gson = new Gson();
        String jsonString = gson.toJson(this);
        // System.out.println(jsonString);
        return jsonString;
    }
}
