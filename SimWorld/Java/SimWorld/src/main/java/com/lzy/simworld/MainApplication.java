package com.lzy.simworld;

import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class MainApplication
{
    public static void main(String[] args)
    {
        SimWorld simWorld = new SimWorld();
        // 连接手机和Matlab(阻塞) + 初始化工作
        simWorld.initSimWorld();

        // 一段延时等待初始化彻底完成
        System.out.println("Waiting for the initial process ...\n\n");
        try
        { Thread.sleep(5000); }
        catch (InterruptedException e)
        { e.printStackTrace(); }

        //
        simWorld.runPhoneSensorDataReceiverThread();

        // 开启任务分配线程
        simWorld.runTaskAllocateThread();

        // 开启快照拍摄线程
        // TODO：快照
        simWorld.runSnapshotThread();


        // TODO 结束全部
    }
}
