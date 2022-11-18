package com.lzy.simworld;

import com.google.gson.Gson;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;

public class SimCarPhoneCar extends SimCar
{
    private String carID = "";  // 车辆ID
    private int carType = Consts.TYPE_PHONE_CAR;  // 车辆类型，默认为非matlab、非phone仿真车
    private final boolean carIsBusy = true;  // 车辆是否正执行任务（false--vacant true--busy）
    private int[] carColor = new int[3];  // 车辆颜色
    private Task carTask = null;
    private CarMotionState carMotionState = null;
    private int carVelocityGridPerSec = 5;  // 汽车每秒运行的格子数（只有非matlab非phone的车需要）
    private PlannedPath carWholePath = null;

    private PhoneSensorData carLastPhoneSensorData = null;  // 上次获取传感器数据事件

    public SimCarPhoneCar(String carID, Coordinate originCoord)
    {
        this.carID = "PhoneCar-" + carID;
        this.carType = Consts.TYPE_PHONE_CAR;
        // 一直繁忙不接受任务，故carIsBusy一直为true
        this.carColor = ToolFunctions.getRandomRGBColor();
        // 将初始点经过调整加入路径中
        this.carWholePath = new PlannedPath(originCoord.getMagnifiedCoord(Consts.PHONE_CAR_COORD_MAGNIFY_RATE));
    }

    // 通过传感器数据计算出手机车的新位置，并添加到汽车路径中
    public void updateCarPositionWithSensorData(PhoneSensorData newPhoneSensorData)
    {
        if(this.carLastPhoneSensorData == null)  // 第一次接收传感器信息
        {
            this.carLastPhoneSensorData = newPhoneSensorData;  // 只更新
        }
        else  // 不是第一次接收传感器信息
        {
            // TODO 使用相邻两次的PhoneSensorData计算位置
            float dTime = (float)(newPhoneSensorData.currentTime - this.carLastPhoneSensorData.currentTime)/1000.0f;  // 毫秒转秒
            // rotAngle转换为弧度制，因为Math库的cos、sin都是接收弧度
            float rotAngle = (float) Math.toRadians(newPhoneSensorData.rotationAnglesValueDegree[0]);  // rotationAnglesValueDegree第一个是绕z轴转动
            float accelerate = newPhoneSensorData.accelerometerSensorValue[0];  // 获取前进的加速度
            Coordinate nextCoord = new Coordinate(this.carWholePath.getPath().get(this.carWholePath.getPathLength()-1));
            // 计算下一个点的位置（公式是时间差*rotAngle）
            nextCoord.row += (int)(0.5*dTime*Math.cos(rotAngle)*accelerate*Consts.PHONE_CAR_COORD_MAGNIFY_RATE*7f);
            nextCoord.col += (int)(0.5*dTime*Math.sin(rotAngle)*accelerate*Consts.PHONE_CAR_COORD_MAGNIFY_RATE*7f);
            this.carWholePath.appendOneCoord(nextCoord);
            // System.out.println(nextCoord.getCoordinateInfoString());
            this.carLastPhoneSensorData = newPhoneSensorData;  // 更新存储在模拟手机车中的传感器信息
        }
    }

    public void updateCarPositionWithSensorData(String dataString)
    {
        PhoneSensorData phoneSensorData = new PhoneSensorData(dataString);
        updateCarPositionWithSensorData(phoneSensorData);
    }


    public static void main(String[] args)
    {
        String tempData1 = "{\"currentTime\":1662389934029,\"rotationAnglesValueDegree\":[170.22226,-21.78766,-0.8998502]," +
                "\"accelerometerSensorValue\":[0.1464198,3.7267282,9.322159],\"magneticFieldSensorValue\":[-9.875,-59.9375,6.0625]," +
                "\"gyroscopeSensorValue\":[0.1430821,-0.11506956,-0.065764]}";
        String tempData2 = "{\"currentTime\":1662389934086,\"rotationAnglesValueDegree\":[180.16525,-18.848991,-2.6695745]," +
                "\"accelerometerSensorValue\":[0.46339294,3.396472,9.938379],\"magneticFieldSensorValue\":[-9.875,-59.9375,6.0625]," +
                "\"gyroscopeSensorValue\":[-0.052307516,-0.56340975,-0.26640704]}";

        // PhoneSensorData phoneSensorData1 = new PhoneSensorData(tempData1);
        // PhoneSensorData phoneSensorData2 = new PhoneSensorData(tempData2);

        SimCarPhoneCar simCarPhoneCar = new SimCarPhoneCar("hello", new Coordinate(11,5));
        System.out.println(simCarPhoneCar.getCarWholePath().retrieveCoordinateByPointer(0).getCoordinateInfoString());
        System.out.println("fuck" + simCarPhoneCar.getCurrentLocationCoord().getCoordinateInfoString());
        simCarPhoneCar.updateCarPositionWithSensorData(tempData1);
        simCarPhoneCar.updateCarPositionWithSensorData(tempData2);
        System.out.println(simCarPhoneCar.getCarWholePath().retrieveCoordinateByPointer(1).getCoordinateInfoString());
        // System.out.println(simCarPhoneCar.getCarWholePath().retrieveCoordinateByPointer(1).getCoordinateInfoString());

        System.out.println("fuck");
        System.out.println(simCarPhoneCar.getCurrentLocationCoord().getCoordinateInfoString());
    }


    // @Override
    // CarMotionState getCurrentMotionState()
    // {
    //     return null;
    // }

    @Override
    Coordinate getCurrentLocationCoord()
    {
        if(this.carWholePath.getPathLength()!=0)
        {
            return this.carWholePath.retrieveCoordinateByPointer(this.carWholePath.getPathLength()-1);
        }
        else
        {
            return null;
        }
    }

    // @Override
    // int getCurrentOrientation()
    // {
    //     return 0;
    // }

    @Override
    boolean acceptNewTask(WorldMap worldMap, Task task)  // 手机仿真车不接受任务
    {
        System.out.println(this.getCarID() + " is busy!");
        return false;
    }

    @Override
    void printCarInfo()
    {
        System.out.println("    carID                   :   "+this.carID);
        System.out.println("    carType                 :   "+this.carType);
        System.out.println();
    }

    @Override
    public String getCarID()
    {
        return this.carID;
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
        return this.carWholePath;
    }

    @Override
    public int getCarWholePathPointer()
    {
        return this.carWholePath.getPathPointer();
    }

    @Override
    public int getCarWholePathLength()
    {
        return this.carWholePath.getPathLength();
    }

    @Override
    public boolean updateCarWholePathPointer()
    {
        return false;
    }

    @Override
    public PlannedPath getCarSubPathByPointers(final int startPointer, final int endPointer)
    {
        return this.carWholePath.getSubPathByPointers(startPointer,endPointer);
    }

    public Coordinate retrieveHistoryLocationCoord(final int retrievePointer)
    { return this.carWholePath.retrieveCoordinateByPointer(retrievePointer); }
}



// 手机车辆运行线程
class PhoneCarRunThread implements SimCarRunThread
{
    private SimCarPhoneCar phoneCar;
    public int startPointer = 0;  // 快照起始位置记录指针
    public int endPointer;  // 快照终止位置记录指针
    public String simCarTrimmedInfoInJson;

    private CountDownLatch countDownLatch = null;

    // 初始化
    public PhoneCarRunThread(SimCarPhoneCar outerSimCar)
    {
        this.phoneCar = outerSimCar;
    }

    @Override
    public void run()
    {
        // 检查countDownLatch是否设定
        if(this.countDownLatch==null)
        { System.out.println(Consts.WARNING_INFO + this.phoneCar.getCarID() + "'s counterDownLatch hasn't been set."); }

        // 主要线程逻辑部分
        int pathEndPointer = this.phoneCar.getCarWholePathLength();
        this.endPointer = pathEndPointer;  // 当前长度作为

        // 打印路径信息
        if(this.startPointer==this.endPointer)
        {
            System.out.println("呆在原地");
        }
        else
        {
            System.out.println(this.phoneCar.getCarID()+" goes from "
                    + this.phoneCar.retrieveHistoryLocationCoord(startPointer).getCoordinateInfoString()
                    + " Pointer:(" + startPointer +") to "
                    + this.phoneCar.retrieveHistoryLocationCoord(endPointer-1).getCoordinateInfoString()
                    + " Pointer:(" + (endPointer-1) +")");
        }

        System.out.println();
        this.simCarTrimmedInfoInJson = new SimCarTrimmedInfo(this.phoneCar, this.startPointer, this.endPointer).packInfoToJson();

        System.out.println("Get Phone Car Trimmed Info Data");
        System.out.println(this.simCarTrimmedInfoInJson);

        this.startPointer = pathEndPointer;  // 下一次的起点是这一次的终点
        // 下面不应该有东西
        // 完成信息记录后计数器自减
        this.countDownLatch.countDown();
        this.countDownLatch = null;  // 清空countDownLatch
    }

    public void setCountDownLatch(CountDownLatch outerCountDownLatch)
    { this.countDownLatch = outerCountDownLatch; }

    public String getSimCarTrimmedInfoInJson()
    { return simCarTrimmedInfoInJson; }
}


// 手机传感器数据接收线程，接收数据并转换为坐标放入模拟手机车的path中
class PhoneSensorDataReceiverThread implements Runnable
{
    // socket相关
    private Socket phoneSocket = null;
    private InputStream inputStream = null;
    private BufferedReader bufferedReader = null;
    // 汽车相关
    private SimCarPhoneCar phoneCar;

    // 构造函数
    public PhoneSensorDataReceiverThread(SimCarPhoneCar outerSimCarPhoneCar, Socket outerSocket)
    {
        System.out.println(Consts.SOCKET_INFO + "Creating Phone Sensor Data Receiver Thread.");
        this.phoneSocket = outerSocket;
        this.phoneCar = outerSimCarPhoneCar;
        // 同时获取相关的InputStream和BufferedReader
        try
        {
            this.inputStream = this.phoneSocket.getInputStream();
            this.bufferedReader = new BufferedReader(new InputStreamReader(this.inputStream));
            // System.out.println(this.phoneCar.getCurrentLocationCoord());
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }


    @Override
    public void run()
    {
        // 先检查是否已经获取到对应socket
        if(this.phoneSocket == null)
        {
            System.out.println(Consts.WARNING_INFO + "The Socket is NULL !!");
            return;
        }
        String sensorDataString = "";
        // int counter = 0;
        try
        {
            while ((sensorDataString = bufferedReader.readLine())!=null)
            {
                // System.out.println("Receive : " + sensorDataString);
                this.phoneCar.updateCarPositionWithSensorData(sensorDataString);
                // System.out.println("counter: " + counter + this.phoneCar.getCurrentLocationCoord().getCoordinateInfoString());
                // counter ++ ;
            }
            System.out.println("Phone Car Final Length : " + this.phoneCar.getCarWholePathLength());
            this.shutDownSocket();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    private void shutDownSocket()
    {
        System.out.println(Consts.SOCKET_INFO + "Shut Down Phone Socket...");
        if(this.bufferedReader!=null)
        {
            try
            {
                this.bufferedReader.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
        if(this.inputStream!=null)
        {
            try
            {
                this.inputStream.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
        if(this.phoneSocket!=null)
        {
            try
            {
                this.phoneSocket.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
    }


    public static void main(String[] args)
    {
        ServerSocket serverSocket = null;
        Socket acceptSocket = null;
        PhoneSensorDataReceiverThread phoneSensorDataReceiverThread;
        SimCarPhoneCar simCarPhoneCar = null;

        try
        {
            serverSocket = new ServerSocket(9999);
            System.out.println("Server Socket is Ready.");
            acceptSocket = serverSocket.accept();
            System.out.println("Phone Socket is Connected.");
            simCarPhoneCar = new SimCarPhoneCar("sbCar", new Coordinate(3,4));
            phoneSensorDataReceiverThread = new PhoneSensorDataReceiverThread(simCarPhoneCar, acceptSocket);
            new Thread(phoneSensorDataReceiverThread).start();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}

class PhoneSensorData
{
    public long currentTime;
    public float[] rotationAnglesValueDegree = new float[3];  // 角度传感器
    public float[] accelerometerSensorValue = new float[3];  // 加速度传感器
    public float[] magneticFieldSensorValue = new float[3];  // 地磁传感器数据
    public float[] gyroscopeSensorValue = new float[3];  // 陀螺仪传感器

    // 构造函数
    public PhoneSensorData() { }
    public PhoneSensorData(final String receivedString)
    {
        Gson gson = new Gson();
        PhoneSensorData tempPhoneSensorsData = gson.fromJson(receivedString, PhoneSensorData.class);
        this.currentTime = tempPhoneSensorsData.currentTime;
        this.rotationAnglesValueDegree = tempPhoneSensorsData.rotationAnglesValueDegree;
        this.accelerometerSensorValue = tempPhoneSensorsData.accelerometerSensorValue;
        this.magneticFieldSensorValue = tempPhoneSensorsData.magneticFieldSensorValue;
        this.gyroscopeSensorValue = tempPhoneSensorsData.gyroscopeSensorValue;
    }

    // 将JSON字符串解析为PhoneSensorData对象（本质是利用构造函数创建一个新的对象并返回）
    public static PhoneSensorData parsePhoneSensorDataJsonString(final String jsonString)
    { return(new PhoneSensorData(jsonString)); }

    // 将传感器信息打包为JSON字符串
    public String getPhoneSensorDataJsonString()
    {
        Gson gson = new Gson();
        String jsonStringSensorData = gson.toJson(this, PhoneSensorData.class);
        System.out.println("Sensors Data in Json : " + jsonStringSensorData);
        return jsonStringSensorData;
    }

    // 展示PhoneSensorData对象的全部信息
    public void showAllPhoneSensorData()
    {
        System.out.println("currentTime               : " + this.currentTime);
        System.out.println("rotationAnglesValueDegree : " + Arrays.toString(this.rotationAnglesValueDegree));
        System.out.println("accelerometerSensorValue  : " + Arrays.toString(this.accelerometerSensorValue));
        System.out.println("magneticFieldSensorValue  : " + Arrays.toString(this.magneticFieldSensorValue));
        System.out.println("gyroscopeSensorValue      : " + Arrays.toString(this.gyroscopeSensorValue));
    }


    // // 测试JSON打包和解析
    // public static void main(String[] args)
    // {
    //     String tempString = "{\"currentTime\":1662193976704,\"rotationAnglesValueDegree\":[141.67574,0.500504,-0.77325064]" +
    //             ",\"accelerometerSensorValue\":[0.13291651,-0.08603803,9.848156],\"magneticFieldSensorValue\":" +
    //             "[-4.0625,-4.25,-34.4375],\"gyroscopeSensorValue\":[-0.0052185343,0.0032637657,-0.0015358897]}";
    //
    //     System.out.println(new PhoneSensorData(tempString).getPhoneSensorDataJsonString());
    //
    //     PhoneSensorData tempData = PhoneSensorData.parsePhoneSensorDataJsonString(tempString);
    //     tempData.showAllPhoneSensorData();
    // }
}

