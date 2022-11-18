package com.lzy.simworld;

public class Consts
{
    // =============== 仿真世界相关 ===============
    public static final int CAR_NUMS = 5;  // 汽车总数
    public static final int TASK_NUMS = 10;  // 预设任务总数
    public static final String SNAPSHOT_FILENAME = "SimWorldSnapshot_"+ System.currentTimeMillis()+ ".txt";

    // =============== Socket通信相关 ===============
    public static final int SERVER_SOCKET_PORT = 9999;
    public static final int PHONE_SERVER_SOCKET_PORT = 9999;
    public static final int MATLAB_SERVER_SOCKET_PORT = 9999;


    // =============== 多线程相关 ===============
    public static final int THREAD_POOL_SIZE = 8;  // 线程池大小
    public static final int ALLOCATE_TASK_THREAD_TIMER_DELAY = 1000;  // 任务分配线程延迟执行时间ms
    public static final int ALLOCATE_TASK_THREAD_TIMER_PERIOD = 3000;  // 任务分配线程执行周期时间ms
    public static final int SNAPSHOT_THREAD_TIMER_DELAY = 2000;  // 快照拍摄线程延迟执行时间ms
    public static final int SNAPSHOT_THREAD_TIMER_PERIOD = 3000;  // 快照拍摄线程执行周期时间ms




    // =============== 输出信息相关 ===============
    public static final String PROMPT_BAR = "=========================";
    public static final String PROMPT_TRIMMED_INFO = "[TRIMMED INFO] : ";
    public static final String PROMPT_SNAPSHOT_INFO = "[SNAPSHOT INFO] : ";
    public static final String PROMPT_TASK_ALLOCATE_INFO = "[TASK ALLOCATE INFO] : ";
    public static final String WARNING_INFO = "[WARNING INFO] : ";
    public static final String SOCKET_INFO = "[SOCKET INFO] : ";


    // =============== 汽车相关 ===============
    public static final int TYPE_PHONE_CAR = 0;
    public static final String TYPE_PHONE_CAR_STRING = "PHONE_CAR";
    public static final int TYPE_MATLAB_CAR = 1;
    public static final String TYPE_MATLAB_CAR_STRING = "MATLAB_CAR";
    public static final int TYPE_VIRTUAL_CAR = 2;
    public static final String TYPE_VIRTUAL_CAR_STRING = "VIRTUAL_CAR";

    public static final int PHONE_CAR_COORD_MAGNIFY_RATE = 1000;  // 手机模拟车坐标放大倍数
    public static final int MATLAB_CAR_COORD_MAGNIFY_RATE = 1000;  // matlab模拟车坐标放大倍数


    // =============== 路径搜索相关 ===============
    public static final int CHOSEN_DIRECTIONS_NUM = Consts.FOUR_DIRECTIONS_NUM;
    public static final int CHOSEN_DISTANCE_CALCULATE_METHOD = Consts.USE_MANHATTAN_DISTANCE;

    // 路径方向
    // {{-1,-1},{-1, 0},{-1, 1},
    //  { 0,-1},        { 0, 1},
    //  { 1,-1},{ 1, 0},{ 1, 1}};
    public static final int[][] EIGHT_DIRECTIONS =
            {{-1,-1},{-1, 0},{-1, 1},
             { 0,-1},        { 0, 1},
             { 1,-1},{ 1, 0},{ 1, 1}};
    // 路径代价
    // {3,2,3,
    //  2,  2,
    //  3,2,3};
    public static final int[] EIGHT_DIRECTIONS_COST =
            {3,2,3,
             2,  2,
             3,2,3};
    public static final int EIGHT_DIRECTIONS_NUM = 8;

    // 路径方向
    // {       {-1, 0} ,
    //  { 0,-1},        { 0, 1},
    //         { 1, 0}        };
    public static final int[][] FOUR_DIRECTIONS =
            {        {-1, 0},
             { 0,-1},       { 0, 1},
                     { 1, 0}       };
    // 路径代价
    // {  2,
    //  2,  2,
    //    2, };
    public static final int[] FOUR_DIRECTIONS_COST =
            {  2,
             2,  2,
               2, };
    public static final int FOUR_DIRECTIONS_NUM = 4;

    public static final int USE_MANHATTAN_DISTANCE = 0;
    public static final int USE_EUCLIDEAN_DISTANCE = 1;

    //
    public static final int IN_OPEN_LIST = 1;
    public static final int IN_CLOSE_LIST = 2;
    public static final int IN_UNKNOWN_LIST = 0;

    public static final int A_STAR_AIM_WEIGHT = 2;
}
