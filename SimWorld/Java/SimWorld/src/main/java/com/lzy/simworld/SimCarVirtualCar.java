package com.lzy.simworld;

import java.util.concurrent.CountDownLatch;

public class SimCarVirtualCar extends SimCar
{
    private String carID = "";  // 车辆ID
    private int carType = 0;  // 车辆类型，默认为非matlab、非phone仿真车
    private boolean carIsBusy = false;  // 车辆是否正执行任务（false--vacant true--busy）
    private int[] carColor = new int[3];  // 车辆颜色
    private Task carTask = null;
    private CarMotionState carMotionState = null;
    private int carVelocityGridPerSec = 5;  // 汽车每秒运行的格子数（只有非matlab非phone的车需要）
    private PlannedPath carWholePath = null;

    // private VirtualCarRunThread carRunThread;


    // 构造函数
    public SimCarVirtualCar(String carID, Coordinate originCoord)
    {
        this.carID = "VirtualCar-" + carID;
        this.carType = Consts.TYPE_VIRTUAL_CAR;
        this.carIsBusy = false;
        this.carColor = ToolFunctions.getRandomRGBColor();
        this.carWholePath = new PlannedPath(originCoord);  // 调用构造函数时会自动将起点加入

        // carRunThread = new VirtualCarRunThread(this);
    }


    // 设置模拟车辆运行速度，一秒可以走几个栅格点
    public void setVelocityGridPerSec(int velocityGridPerSec)
    { this.carVelocityGridPerSec = velocityGridPerSec; }

    // public VirtualCarRunThread getVirtualCarRunThread(CountDownLatch outerCountDownLatch)
    // {
    //     this.carRunThread.setCountDownLatch(outerCountDownLatch);
    //     System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "Get " + this.carID + "'s Run Thread.");
    //     return carRunThread;
    // }

    // // 获取车辆运行线程
    // public VirtualCarRunThread getVirtualCarRunThread()
    // {
    //     System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "Get " + this.carID + "'s Run Thread.");
    //     return carRunThread;
    // }

    public CarMotionState getCurrentMotionState()
    {
        return this.carMotionState;
    }

    // 获取车辆当前位置
    public Coordinate getCurrentLocationCoord()
    {
        if(this.carWholePath.getPathPointer()==0)
        {
            return this.carWholePath.getPath().get(this.carWholePath.getPathPointer());
        }
        else
        {
            return this.carWholePath.getPath().get(this.carWholePath.getPathPointer()-1);
        }
    }

    // 查询历史路径坐标
    public Coordinate retrieveHistoryLocationCoord(final int retrievePointer)
    { return this.carWholePath.retrieveCoordinateByPointer(retrievePointer); }

    // 获取车辆当前位置
    public int getCurrentOrientation()
    {
        //TODO
        return this.carMotionState.getOrientation();
    }

    // 车辆接收新任务：
    // 如果车辆繁忙，则不接受新任务，返回false。
    // 如果车辆空闲，则接收新任务，返回true。添加两端路径：①从车辆当前位置去往任务起点②任务本身的路径
    public boolean acceptNewTask(final WorldMap worldMap, Task task)
    {
        if(this.carIsBusy)
        {
            System.out.println(this.getCarID() + " is busy!");
            return false;
        }
        else
        {
            System.out.println(this.getCarID() + " is not busy!");
            this.carTask = task;  // 分配任务
            this.carIsBusy = true;
            // 获取从车辆当前位置到达新任务起点的路径
            Coordinate carCurrentCoord = this.getCurrentLocationCoord();
            Coordinate newTaskOriginCoord = task.getTaskOriginCoord();
            PlannedPath goToNewTaskOriginPath = new PlannedPath(carCurrentCoord, newTaskOriginCoord);
            goToNewTaskOriginPath.pathPlanningAStar(worldMap, Consts.CHOSEN_DIRECTIONS_NUM, Consts.CHOSEN_DIRECTIONS_NUM);
            System.out.print("添加去往任务起点的路径     :  ");
            this.carWholePath.pathMerge(goToNewTaskOriginPath);
            System.out.print("添加任务路径             :  ");
            this.carWholePath.pathMerge(task);
            this.carWholePath.setDestination(task.getTaskPath().getDestination());
            return true;
        }
    }

    //
    public void printCarInfo()
    {
        // private int carVelocityGridPerSec = 5;  // 汽车每秒运行的格子数（只有非matlab非phone的车需要）
        System.out.println("    carID                   :   "+this.carID);
        System.out.println("    carType                 :   "+this.carType);
        // System.out.println("    carIsBusy               :   "+this.carIsBusy);
        // System.out.println("carColor                :   "+ Arrays.toString(this.carColor));
        // if(carTask!=null)
        //     System.out.println("carTask                 :   "+this.carTask.getTaskID());
        // else
        //     System.out.println("carTask                 :   "+"No Task.");
        // System.out.println("carMotionState          :   "+this.carMotionState.getCarMotionStateJsonString());
        // System.out.println("carVelocityGridPerSec   :   "+this.carVelocityGridPerSec);
        System.out.println();
    }

    public String getCarID()
    { return carID; }

    public int getCarType()
    { return carType; }

    public boolean isCarIsBusy()
    { return carIsBusy; }

    public int[] getCarColor()
    { return carColor; }

    public Task getCarTask()
    { return carTask; }

    public CarMotionState getCarMotionState()
    { return carMotionState; }

    public int getCarVelocityGridPerSec()
    { return carVelocityGridPerSec; }

    public PlannedPath getCarWholePath()
    { return carWholePath; }

    // 获取车辆总路径位置指针
    public int getCarWholePathPointer()
    {
        return this.carWholePath.getPathPointer();
    }

    // 获取车辆总路径长度
    public int getCarWholePathLength()
    {
        return this.carWholePath.getPath().size();
    }

    // 返回是否有移动（没移动说明本身就是终点）
    public boolean updateCarWholePathPointer()
    {
        // 使用车辆速度(每秒前进多少格子)
        boolean moveFlag = this.carWholePath.updatePathPointer(this.carVelocityGridPerSec);
        // 如果没有移动，说明车辆完成任务，设为空闲
        if(!moveFlag)
        {
            this.carTask = null;  // 清空任务
            this.carIsBusy = false;  // 车辆设为空闲
        }
        return moveFlag;
    }

    public PlannedPath getCarSubPathByPointers(final int startPointer, final int endPointer)
    {
        return this.carWholePath.getSubPathByPointers(startPointer,endPointer);
    }
}


// 车辆任务执行线程
class VirtualCarRunThread implements SimCarRunThread
{
    private SimCarVirtualCar virtualCar;
    public int startPointer;
    public int endPointer;
    private String simCarTrimmedInfoInJson;
    private CountDownLatch countDownLatch = null;

    public VirtualCarRunThread(SimCarVirtualCar outerSimCar)
    {
        this.virtualCar = outerSimCar;
    }

    @Override
    public void run()
    {
        // 检查countDownLatch是否设定
        if(this.countDownLatch==null)
        { System.out.println(Consts.WARNING_INFO + this.virtualCar.getCarID() + "'s counterDownLatch hasn't been set."); }
        // else
        // { System.out.println(Consts.WARNING_INFO + this.virtualCar.getCarID() + "'s counterDownLatch has been set."); }

        this.startPointer = this.virtualCar.getCarWholePathPointer();
        this.virtualCar.updateCarWholePathPointer();  // 模拟汽车前进，同时更新wholePathPointer
        this.endPointer = this.virtualCar.getCarWholePathPointer();

        // 打印路径信息
        if(this.startPointer==this.virtualCar.getCarWholePathLength())
        {
            System.out.println("呆在原地");
        }
        else
        {
            System.out.println(this.virtualCar.getCarID()+" goes from "
                    + this.virtualCar.retrieveHistoryLocationCoord(startPointer).getCoordinateInfoString()
                    + " Pointer:(" + startPointer +") to "
                    + this.virtualCar.retrieveHistoryLocationCoord(endPointer-1).getCoordinateInfoString()
                    + " Pointer:(" + (endPointer-1) +")");
        }

        this.simCarTrimmedInfoInJson = new SimCarTrimmedInfo(this.virtualCar, this.startPointer, this.endPointer).packInfoToJson();
        System.out.println(Consts.PROMPT_TRIMMED_INFO + this.simCarTrimmedInfoInJson);
        this.countDownLatch.countDown();
        this.countDownLatch = null;  // 清空countDownLatch
    }

    public String getSimCarTrimmedInfoInJson()
    { return simCarTrimmedInfoInJson; }

    public void setCountDownLatch(CountDownLatch outerCountDownLatch)
    { this.countDownLatch = outerCountDownLatch; }
}
