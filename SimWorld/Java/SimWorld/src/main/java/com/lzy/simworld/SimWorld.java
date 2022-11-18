package com.lzy.simworld;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Date;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class SimWorld
{
    private WorldMap simWorldMap = new WorldMap(0,0);
    private SimCar[] simCarsList = new SimCar[5];  // 0号为手机传感器，1号为Matlab，2、3、4为仿真自主车
    private ArrayList<Task> tasksList = new ArrayList<Task>(8);
    private int tasksListPointer = 0;  // 用于指明，当前执行到第几个任务了
    private SocketGroup socketGroup = null;

    // private Timer simWorldTimer = new Timer("SimWorldTimer");


    // 定时任务线程池"入口"，不是严格按照period执行（运行时间>period，两次运行紧挨；运行时间<period，按照period执行）
    private ScheduledExecutorService scheduledThreadPool = Executors.newScheduledThreadPool(Consts.THREAD_POOL_SIZE);
    // private Runnable[] simCarRunThreadList = new Runnable[Consts.CAR_NUMS];

    // 传感器信息信息线程
    // private PhoneSensorDataReceiverThread phoneSensorDataReceiverThread = null;

    // SimWorld(){}

    // simWorld初始化
    // 地图创建  任务创建
    public void initSimWorld()
    {
        System.out.println("\n\n" + Consts.PROMPT_BAR + "开始进行SimWorld初始化    时间："+ new Date() + Consts.PROMPT_BAR);
        // 地图创建
        System.out.println("地图创建...\n");
        // this.simWorldMap.generateFixedSimpleMap();
        // this.simWorldMap.generateFixedMap();
        this.simWorldMap.generateSmallerGuoMap();
        System.out.println("创建的地图JSON信息如下");
        System.out.println(this.simWorldMap.getMapJsonInfo());

        // 任务创建
        System.out.println("任务创建：共"+Consts.TASK_NUMS+"个...");
        for(int i = 0;i < Consts.TASK_NUMS; i++)
        {
            System.out.format("创建第%3d 个任务\n",i);
            Task tempTask = createOneRandomTask(true);
            // System.out.println(tempTask.getTaskJsonString());
            tasksList.add(tempTask);
        }
        // 车辆创建
        System.out.println("车量创建：共"+Consts.CAR_NUMS+"辆...");
        for(int i=0;i<Consts.CAR_NUMS;i++)
        {
            System.out.format("创建第%3d 个车辆\n",i);
            String tempCarID = String.valueOf(i);
            if(i==0)
            {
                simCarsList[0] = new SimCarPhoneCar(tempCarID, simWorldMap.provideRandomVacantCoordinate());
            }
            else
            {
                simCarsList[i] = new SimCarVirtualCar(tempCarID ,simWorldMap.provideRandomVacantCoordinate());
            }
            simCarsList[i].printCarInfo();
        }

        // TODO socket 连接
        System.out.println("连接所需的服务端");
        this.socketGroup = new SocketGroup();
        // System.out.println("创建相关信息通讯线程");
        // this.phoneSensorDataReceiverThread = new PhoneSensorDataReceiverThread((SimCarPhoneCar)this.simCarsList[0], this.socketGroup.phoneSocket);


        System.out.println(Consts.PROMPT_BAR + "SimWorld初始化完成" + Consts.PROMPT_BAR + "\n\n");
        // new Thread(new TaskAllocateThread(this), "TaskAllocateThread").start();
    }

    // 运行任务分配线程的API
    public void runTaskAllocateThread()
    {
        TaskAllocateThread allocateTaskTimerTask = new TaskAllocateThread(this);
        // 任务创建完毕后加入线程池
        this.scheduledThreadPool.scheduleAtFixedRate(allocateTaskTimerTask,
                Consts.ALLOCATE_TASK_THREAD_TIMER_DELAY, Consts.ALLOCATE_TASK_THREAD_TIMER_PERIOD, TimeUnit.MILLISECONDS);
    }

    // 运行快照线程API
    public void runSnapshotThread()
    {
        SnapshotThread snapshotThread = new SnapshotThread(this.simCarsList);
        this.scheduledThreadPool.scheduleAtFixedRate(snapshotThread,
                Consts.SNAPSHOT_THREAD_TIMER_DELAY, Consts.SNAPSHOT_THREAD_TIMER_PERIOD, TimeUnit.MILLISECONDS);
    }

    // 运行手机传感器数据接收线程
    public void runPhoneSensorDataReceiverThread()
    {
        PhoneSensorDataReceiverThread phoneSensorDataReceiverThread = new PhoneSensorDataReceiverThread((SimCarPhoneCar)this.simCarsList[0], this.socketGroup.phoneSocket);
        new Thread(phoneSensorDataReceiverThread).start();
    }




    // 创建一个随机任务（保证能够完成，即起点终点是联通的）
    // TODO：还应保证任务起点和汽车当前位置联通
    public Task createOneRandomTask(boolean showTaskInfo)
    {
        // 判断是否为一个合理的可完成的Task
        boolean successFlag = false;

        Coordinate Origin = null;
        Coordinate Destination = null;
        PlannedPath path = null;

        // 不停尝试创建直到找到两个联通的点，可以用于创建task
        while(!successFlag)
        {
            Origin = this.simWorldMap.provideRandomVacantCoordinate();
            Destination = this.simWorldMap.provideRandomVacantCoordinate();
            path = new PlannedPath(Origin,Destination);
            path.pathPlanningAStar(this.simWorldMap, Consts.EIGHT_DIRECTIONS_NUM, Consts.USE_MANHATTAN_DISTANCE);
            if(path.getPathCost()!=-1)
                successFlag = true;
            // else
            //     System.out.println("此次随机产生的任务无法完成");
        }

        Task tempTask = new Task(Origin, Destination, path);

        if(showTaskInfo)
            tempTask.printTaskInfo();

        return tempTask;
    }

    // getters&setters
    public WorldMap getSimWorldMap()
    { return simWorldMap; }

    public SimCar[] getSimCarsList()
    { return simCarsList; }

    public ArrayList<Task> getTasksList()
    { return tasksList; }

    public int getTasksListPointer()
    { return tasksListPointer; }

    public void setTasksListPointer(int tasksListPointer)
    { this.tasksListPointer = tasksListPointer; }

    public void addToTaskListPointer(int addNum)
    { this.tasksListPointer+=addNum; }

    public boolean simWorldTaskListIsEmpty()
    {
        return(this.getTasksListPointer() == this.getTasksList().size());
    }

    public ScheduledExecutorService getScheduledThreadPool()
    {
        return scheduledThreadPool;
    }
}


// 用于各个设备通信（会阻塞，直到客户端都连接）
class SocketGroup
{
    public ServerSocket serverSocket = null;
    public Socket matlabSocket = null;
    public Socket phoneSocket = null;
    public Socket guiSocket = null;

    public SocketGroup()
    {
        try
        {
            System.out.println(Consts.SOCKET_INFO + "ServerSocket is Ready. Waiting for Phone and Matlab to Connect...");
            serverSocket = new ServerSocket(Consts.SERVER_SOCKET_PORT);
            System.out.println(Consts.SOCKET_INFO + "Let PhoneSocket Connect.");
            phoneSocket = serverSocket.accept();
            System.out.println(Consts.SOCKET_INFO + "PhoneSocket is Connected.");
            // System.out.println(Consts.SOCKET_INFO + "Let MatlabSocket Connect.");
            // matlabSocket = serverSocket.accept();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        finally
        {
            if(serverSocket!=null)
            {
                try
                {
                    serverSocket.close();
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args)
    {
        SocketGroup socketGroup = new SocketGroup();
    }
}


// 任务分配线程（定时任务）
class TaskAllocateThread implements Runnable
{
    // 任务分配，手机模拟车不需要接受
    // 其实是将一个任务轮询各个车辆，让他们尝试接收，不行就等着

    private int runTimes = 0;  // 记录分配任务线程执行次数
    private SimWorld simWorld = null;

    public TaskAllocateThread(SimWorld outerSimWorld)
    {
        this.simWorld = outerSimWorld;
    }

    @Override
    public void run()
    {
        // while (simWorld.getTasksListPointer()!= simWorld.getTasksList().size())  // 开启定时任务，无需while循环
        System.out.println(Consts.PROMPT_BAR + "第 "+this.runTimes +" 次任务分配线程启动    时间："+ new Date() + Consts.PROMPT_BAR);
        this.runTimes++;
        // 不主动延时，由Timer控制
        for(int i = 0;i < Consts.CAR_NUMS; i++)
        {
            // 如果没有待分配的任务则跳出
            if(simWorld.getTasksListPointer() == simWorld.getTasksList().size())
            {
                System.out.println("TasksList is Empty. Waiting for New Tasks...");
                break;
            }
            // 如果还有待分配的任务则轮询
            SimCar tempCar = simWorld.getSimCarsList()[i];
            Task tempTask = simWorld.getTasksList().get(simWorld.getTasksListPointer());
            boolean acceptFlag = tempCar.acceptNewTask(simWorld.getSimWorldMap(), tempTask);
            if(acceptFlag)
            {
                System.out.println(tempCar.getCarID()+" accepts Task-"+tempTask.getTaskID());
                simWorld.addToTaskListPointer(1);
            }
            else
            {
                System.out.println(tempCar.getCarID()+" refuses Task-"+tempTask.getTaskID());
            }
        }
        System.out.println();
    }
}

// 实际 车辆运行线程 + 快照 + 与可视化通信（定时任务）
// 通过拍摄快照获取使车辆一段时间内的运行路径
class SnapshotThread implements Runnable
{
    private int runTimes = 0;
    private SimCar[] simCarList;
    private Runnable[] simCarRunThreadList;
    private StringBuffer allSimCarSnapshotInfoInJson;  // 本次快照的全部信息（JSON形式）
    private CountDownLatch countDownLatch;  // 锁，等待全部车辆获取快照信息，再统一传给可视化部分

    // 通过simCarList创建相关的运行线程
    public SnapshotThread(SimCar[] simCars)
    {
        this.simCarList = simCars;
        this.simCarRunThreadList = new Runnable[Consts.CAR_NUMS];
        for (int carIdx = 0; carIdx < Consts.CAR_NUMS; carIdx++)
        {
            if(this.simCarList[carIdx] instanceof SimCarVirtualCar)
            {
                this.simCarRunThreadList[carIdx] = new VirtualCarRunThread((SimCarVirtualCar) this.simCarList[carIdx]);
            }
            else if(this.simCarList[carIdx] instanceof SimCarPhoneCar)
            {
                this.simCarRunThreadList[carIdx] = new PhoneCarRunThread((SimCarPhoneCar) this.simCarList[carIdx]);
            }
            // TODO 其他类型的车
        }
    }

    // 设置各个运行线程的countDownLatch
    private void setAllCarRunThreadLatch()
    {
        for(int carIdx=0; carIdx<Consts.CAR_NUMS;carIdx++)
        {
            if(this.simCarList[carIdx] instanceof SimCarVirtualCar)
            {
                ((VirtualCarRunThread)this.simCarRunThreadList[carIdx]).setCountDownLatch(this.countDownLatch);
            }
            else if(this.simCarList[carIdx] instanceof SimCarPhoneCar)
            {
                ((PhoneCarRunThread)this.simCarRunThreadList[carIdx]).setCountDownLatch(this.countDownLatch);
            }
            // TODO 其他类型的车
        }
    }


    @Override
    public void run()
    {
        System.out.println(Consts.PROMPT_BAR + "第 "+this.runTimes +" 次快照拍摄线程启动    时间："+ new Date() + Consts.PROMPT_BAR);

        this.countDownLatch = new CountDownLatch(Consts.CAR_NUMS);  // 每次运行都需要新的CountDownLatch
        setAllCarRunThreadLatch();  // 重新设置各个线程的countDownLatch

        System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "等待各个车辆拍摄快照中....");

        for(int carIdx=0; carIdx<Consts.CAR_NUMS;carIdx++)
        {
            System.out.println(Consts.PROMPT_SNAPSHOT_INFO + this.simCarList[carIdx].getCarID() + "快照拍摄中");
            new Thread(this.simCarRunThreadList[carIdx]).start();
            System.out.println("开始运行");
        }
        // 等待每个车辆获取完相应的信息，再传给可视化部分
        try
        {
            this.countDownLatch.await();  // 阻塞，直到每一辆车的快照拍摄工作完成
            System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "各个车辆快照拍摄完毕，整合信息中...");
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }

        // TODO 消息打包+传输
        // 获取各个车辆的快照信息
        System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "收集各个车辆的快照信息");

        this.allSimCarSnapshotInfoInJson = new StringBuffer(1024);  // 预计长度
        // this.allSimCarSnapshotInfoInJson.append("[");
        for (int carIdx = 0; carIdx < Consts.CAR_NUMS; carIdx++)
        {
            String tempSnapshotTrimmedInfo = null;
            if(this.simCarList[carIdx] instanceof SimCarVirtualCar)
            {
                tempSnapshotTrimmedInfo = ((VirtualCarRunThread)this.simCarRunThreadList[carIdx]).getSimCarTrimmedInfoInJson();
            }
            else if(this.simCarList[carIdx] instanceof SimCarPhoneCar)
            {
                tempSnapshotTrimmedInfo = ((PhoneCarRunThread)this.simCarRunThreadList[carIdx]).getSimCarTrimmedInfoInJson();
            }
            // TODO 处理其他车辆类型

            this.allSimCarSnapshotInfoInJson.append(tempSnapshotTrimmedInfo);
            // 不是json数组格式
            this.allSimCarSnapshotInfoInJson.append("\n");
        }
        System.out.println(this.allSimCarSnapshotInfoInJson);

        // 运行次数递增
        this.runTimes++;

        // TODO 发送完整JSON给可视化
        System.out.println(Consts.PROMPT_SNAPSHOT_INFO + "发送信息给可视化主机\n");
        this.saveSnapshotToFile();
    }

    private void saveSnapshotToFile()
    {
        long currentTime = System.currentTimeMillis();
        File snapshotsFile = new File(Consts.SNAPSHOT_FILENAME);
        FileOutputStream fileOutputStream = null;
        try
        {
            fileOutputStream = new FileOutputStream(snapshotsFile, true);
            fileOutputStream.write(this.allSimCarSnapshotInfoInJson.toString().getBytes());
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        finally
        {
            if(fileOutputStream!=null)
            {
                try
                {
                    fileOutputStream.close();
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        }
    }
}

