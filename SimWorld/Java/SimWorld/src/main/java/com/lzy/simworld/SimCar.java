package com.lzy.simworld;

import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;

public abstract class SimCar
{
    // /*成员变量*/
    // // 车辆属性
    // private String carID = "";  // 车辆ID
    // private int carType = 0;  // 车辆类型，默认为非matlab、非phone仿真车
    // private boolean carIsBusy = false;  // 车辆是否正执行任务（false--vacant true--busy）
    // // private int carCapacity = 0;  // 车辆容积
    // // private int carPriority = 0;  // 车辆优先级
    // private int[] carColor = new int[3];  // 车辆颜色
    // // private int[] carVisualSize = new int[3];  // 视觉大小
    // // float carCollisionRadius = 0;  // 碰撞圆面积
    // // 两个类
    // private Task carTask = null;
    // private CarMotionState carMotionState = null;

    /*成员函数*/
    // 获取车辆运动状态
    // abstract CarMotionState getCurrentMotionState();
    // 获取车辆当前位置
    abstract Coordinate getCurrentLocationCoord();
    // 获取车辆当前位置
    // abstract int getCurrentOrientation();
    // 车辆接收新任务
    abstract boolean acceptNewTask(WorldMap worldMap, Task task);
    // 打印车辆信息
    abstract void printCarInfo();
    // 车辆前往目标点
    // public abstract Task goToTaskOrigin();
    // getters
    public abstract String getCarID();

    public abstract int getCarType();

    public abstract boolean isCarIsBusy();

    public abstract int[] getCarColor();

    public abstract Task getCarTask();

    public abstract CarMotionState getCarMotionState();

    public abstract int getCarVelocityGridPerSec();

    public abstract PlannedPath getCarWholePath();

    public abstract int getCarWholePathPointer();

    public abstract int getCarWholePathLength();

    public abstract boolean updateCarWholePathPointer();

    public abstract PlannedPath getCarSubPathByPointers(final int startPointer, final int endPointer);
}

interface SimCarRunThread extends Runnable
{
    String getSimCarTrimmedInfoInJson();
    void run();
    void setCountDownLatch(CountDownLatch countDownLatch);
}


// 模拟车精简信息
class SimCarTrimmedInfo
{
    private String id;
    private int type;
    private int[] color;
    private int pathLength;
    private ArrayList<Coordinate> path = new ArrayList<>();

    // 构造函数获取部分路径
    public SimCarTrimmedInfo(final SimCar simCar, final int pathStartPointer, final int pathEndPointer)
    {
        this.id = simCar.getCarID();
        this.type = simCar.getCarType();
        this.color = simCar.getCarColor();
        this.path = simCar.getCarSubPathByPointers(pathStartPointer,pathEndPointer).getPath();
        this.pathLength = this.path.size();
    }

    // 构造函数，获取全部路径
    public SimCarTrimmedInfo(final SimCar simCar)
    {
        this.id = simCar.getCarID();
        this.type = simCar.getCarType();
        this.color = simCar.getCarColor();
        this.path = simCar.getCarWholePath().getPath();
        this.pathLength = this.path.size();
    }


    // 将信息打包成JSON字符串
    public String packInfoToJson()
    {
        // Gson gson = new Gson();
        // String infoJsonString = gson.toJson(this, SimCarTrimmedInfo.class);
        // return infoJsonString;

        StringBuffer buffer = new StringBuffer();
        buffer.append("{\"id\":\""+this.id+"\",\"type\":"+this.type+",\"color\":"+Arrays.toString(this.color)+",");
        buffer.append(this.getPathTrimmedJsonInfo()+"}");
        return String.valueOf(buffer);
    }

    // 调用下方获取精简的路径信息
    public String getPathTrimmedJsonInfo()
    {
        StringBuffer tempBuffer = new StringBuffer();

        int[] rows = null;
        int[] cols = null;
        int length = 0;

        // 同步块，防止读取时被修改
        synchronized (this)
        {
            length = this.pathLength;
            rows = new int[length];
            cols = new int[length];

            for(int i=0;i<this.pathLength;i++)
            {
                rows[i] = this.path.get(i).row;
                cols[i] = this.path.get(i).col;
            }
        }

        tempBuffer.append("\"pathLength\":");
        tempBuffer.append(length);
        tempBuffer.append(",");
        // tempBuffer.append(",\"path\":");
        // tempBuffer.append("{");
        // 添加rows
        tempBuffer.append("\"rows\":");
        tempBuffer.append(Arrays.toString(rows));
        // 添加cols
        tempBuffer.append(",\"cols\":");
        tempBuffer.append(Arrays.toString(cols));
        // 添加结尾
        // tempBuffer.append("}");

        return String.valueOf(tempBuffer);
    }
}