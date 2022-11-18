package com.lzy.simworld;

import java.util.concurrent.CountDownLatch;

public class SimCarMatlabCar extends SimCar
{
    private String carID = "";  // 车辆ID
    private int carType = 0;  // 车辆类型，默认为非matlab、非phone仿真车
    private boolean carIsBusy = false;  // 车辆是否正执行任务（false--vacant true--busy）
    private int[] carColor = new int[3];  // 车辆颜色
    private Task carTask = null;
    private CarMotionState carMotionState = null;
    private PlannedPath plannedPath = null;
    private PlannedPath realPath = null;

    public SimCarMatlabCar(String carID, Coordinate originCoord)
    {
        this.carID = "MatlabCar-"+carID;
        this.carType = Consts.TYPE_MATLAB_CAR;
        this.carIsBusy = false;
        this.carColor = ToolFunctions.getRandomRGBColor();
        this.plannedPath = new PlannedPath(originCoord);
    }


    // @Override
    // CarMotionState getCurrentMotionState()
    // {
    //     return null;
    // }

    @Override
    Coordinate getCurrentLocationCoord()
    {
        return null;
    }

    // @Override
    // int getCurrentOrientation()
    // {
    //     return 0;
    // }

    @Override
    boolean acceptNewTask(WorldMap worldMap, Task task)
    {
        return false;
    }

    @Override
    void printCarInfo()
    {

    }

    @Override
    public String getCarID()
    {
        return null;
    }

    @Override
    public int getCarType()
    {
        return 0;
    }

    @Override
    public boolean isCarIsBusy()
    {
        return false;
    }

    @Override
    public int[] getCarColor()
    {
        return new int[0];
    }

    @Override
    public Task getCarTask()
    {
        return null;
    }

    @Override
    public CarMotionState getCarMotionState()
    {
        return null;
    }

    @Override
    public int getCarVelocityGridPerSec()
    {
        return 0;
    }

    @Override
    public PlannedPath getCarWholePath()
    {
        return null;
    }

    @Override
    public int getCarWholePathPointer()
    {
        return 0;
    }

    @Override
    public int getCarWholePathLength()
    {
        return 0;
    }

    @Override
    public boolean updateCarWholePathPointer()
    {
        return false;
    }

    @Override
    public PlannedPath getCarSubPathByPointers(int startPointer, int endPointer)
    {
        return null;
    }
}

class MatlabCarRunThread implements SimCarRunThread
{
    private SimCarMatlabCar matlabCar;
    public int startPointer;
    public int endPointer;
    public String simCarTrimmedInfoInJson;

    @Override
    public String getSimCarTrimmedInfoInJson()
    {
        return null;
    }

    @Override
    public void run()
    {

    }

    @Override
    public void setCountDownLatch(CountDownLatch countDownLatch)
    {

    }
}

class ToMatlabThread implements Runnable
{

    @Override
    public void run()
    {

    }
}


class FromMatlabThread implements Runnable
{

    @Override
    public void run()
    {

    }
}


