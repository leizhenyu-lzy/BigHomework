package com.lzy.simworld;

import com.google.gson.Gson;

import java.util.ArrayList;

public class Task
{
    private static int tasksNumber;  // 用于计数一共有多少tasksNumber

    private int taskID;  // 任务编号
    // private String taskName;  // 任务名称
    // private String taskType;  // 任务类型
    private long taskStartTime = 0;  // 任务开始时间
    private long taskEndTime = -1;  // 任务结束时间
    // private int taskPriority = 0;  // 任务优先级
    private Coordinate taskOriginCoord = null;  // 任务起始点
    private Coordinate taskDestinationCoord = null;  // 任务终点
    private PlannedPath taskPath = null;


    Task(Coordinate originCoord, Coordinate destinationCoord, PlannedPath path)
    {
        taskStartTime = System.currentTimeMillis();
        taskID = tasksNumber;
        tasksNumber++;  // Task类static计数器++
        this.taskOriginCoord = originCoord;
        this.taskDestinationCoord = destinationCoord;
        this.taskPath = path;
    }


    public void printTaskInfo()
    {
        System.out.println("    taskID                  :   " +taskID);
        // System.out.println("taskStartTime           :   " +taskStartTime);
        System.out.print("    taskOriginCoord         :   " +taskOriginCoord.getCoordinateInfoString());
        System.out.println("    taskDestinationCoord    :   " +taskDestinationCoord.getCoordinateInfoString());
        System.out.println("    taskPathCost            :   " +taskPath.getPathCost());
        System.out.println();
    }

    // 通过Gson获取Coordinate的Json字符串
    public String getTaskJsonString()
    {
        Gson gson = new Gson();
        String jsonString = gson.toJson(this);
        // System.out.println(jsonString);
        return jsonString;
    }


    /*Getters & Setters*/

    public PlannedPath getTaskPath()
    {
        return taskPath;
    }

    public long getTaskEndTime()
    {
        return taskEndTime;
    }

    public void setTaskEndTime(long taskEndTime)
    {
        this.taskEndTime = taskEndTime;
    }

    public Coordinate getTaskOriginCoord()
    {
        return taskOriginCoord;
    }

    public void setTaskOriginCoord(Coordinate taskOriginCoord)
    {
        this.taskOriginCoord = taskOriginCoord;
    }

    public Coordinate getTaskDestinationCoord()
    {
        return taskDestinationCoord;
    }

    public void setTaskDestinationCoord(Coordinate taskDestinationCoord)
    {
        this.taskDestinationCoord = taskDestinationCoord;
    }

    public static int getTasksNumber()
    {
        return tasksNumber;
    }

    public void getTaskOriginAndDestinationCoord(Coordinate outerOriginCoord, Coordinate outerDestinationCoord)
    {
        setTaskOriginCoord(outerOriginCoord);
        setTaskDestinationCoord(outerDestinationCoord);
    }

    public int getTaskID()
    {
        return taskID;
    }
}
