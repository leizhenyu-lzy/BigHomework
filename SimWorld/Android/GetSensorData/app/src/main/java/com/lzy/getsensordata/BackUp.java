//package com.lzy.getsensordata;
//
//import androidx.appcompat.app.AppCompatActivity;
//
//import android.hardware.Sensor;
//import android.hardware.SensorEvent;
//import android.hardware.SensorEventListener;
//import android.hardware.SensorManager;
//import android.os.Bundle;
//import android.view.View;
//import android.widget.Button;
//import android.widget.EditText;
//import android.widget.TextView;
//import android.widget.Toast;
//
//// 手机坐标系：
////      屏幕宽方向：x
////      屏幕长方向：y
////      屏幕高方向：z
//// 加速度：①左右x，②前后y，③上下z（平放在桌面上）
//// 旋转角：①绕z轴旋转，②绕x轴旋转，③绕y轴旋转
//
//
//
//public class MainActivity extends AppCompatActivity
//{
//    // 全部可交互控件
//    AllComponents allComponents;
//    Button buttonConnect, buttonStart, buttonStop;
//    TextView textViewAX,textViewAY,textViewAZ,textViewRX,textViewRY,textViewRZ;
//    EditText editTextServerIP, editTextServerPort;
//
//
//    // 全局信息
//    boolean sendSensorDataFlag = false;
//    String serverIP = "192.168.0.106";
//    int serverPort = 9999;
//    //PhoneSensorsData phoneSensorsData = new PhoneSensorsData();
//
//    // 传感器相关
//    //private SensorManager sensorManager;  // 定义传感器管理器
//    //Sensor gyroscopeSensor, accelerometerSensor, magneticFieldSensor;  // 创建三个传感器
//    //MyPhoneSensorsEventListener gyroscopeSensorEventListener, accelerometerSensorEventListener,magneticFieldSensorEventListener;
//
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState)
//    {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
//        // 上面为自带的，下面开始是自己的内容
//
//        // 首先关联控件(R表示res目录下的)
//        //allComponents.buttonConnect = findViewById(R.id.buttonConnect);
//        //allComponents.buttonStart = findViewById(R.id.buttonStart);
//        //allComponents.buttonStop = findViewById(R.id.buttonStop);
//        //allComponents.textViewAX = findViewById(R.id.textViewAX);
//        //allComponents.textViewAY = findViewById(R.id.textViewAY);
//        //allComponents.textViewAZ = findViewById(R.id.textViewAZ);
//        //allComponents.textViewRX = findViewById(R.id.textViewRX);
//        //allComponents.textViewRY = findViewById(R.id.textViewRY);
//        //allComponents.textViewRZ = findViewById(R.id.textViewRZ);
//        //allComponents.textViewTime = findViewById(R.id.textViewTime);
//        //allComponents.editTextServerIP = findViewById(R.id.editTextServerIP);
//        //allComponents.editTextServerPort = findViewById(R.id.editTextServerPort);
//
//        buttonConnect = findViewById(R.id.buttonConnect);
//        buttonStart = findViewById(R.id.buttonStart);
//        buttonStop = findViewById(R.id.buttonStop);
//        textViewAX = findViewById(R.id.textViewAX);
//        allComponents.textViewAY = findViewById(R.id.textViewAY);
//        allComponents.textViewAZ = findViewById(R.id.textViewAZ);
//        allComponents.textViewRX = findViewById(R.id.textViewRX);
//        allComponents.textViewRY = findViewById(R.id.textViewRY);
//        allComponents.textViewRZ = findViewById(R.id.textViewRZ);
//        allComponents.textViewTime = findViewById(R.id.textViewTime);
//        allComponents.editTextServerIP = findViewById(R.id.editTextServerIP);
//        allComponents.editTextServerPort = findViewById(R.id.editTextServerPort);
//
//
//        // 设定初始值
//        allComponents.editTextServerIP.setText(serverIP);
//        allComponents.editTextServerPort.setText(String.valueOf(serverPort));
//
//        //// 传感器相关操作
//        //sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
//        //// 获取相关传感器
//        //magneticFieldSensor = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
//        //accelerometerSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
//        //gyroscopeSensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
//
//
//
//
//        // 设置控件的一些相关Listener
//        // Connect按钮的点击功能：获取输入的主机地址，打印相关信息
//        allComponents.buttonConnect.setOnClickListener(new View.OnClickListener()
//        {
//            @Override
//            public void onClick(View view)
//            {
//                // 从editText中读取服务端信息，并更新信息
//                serverIP = allComponents.editTextServerIP.getText().toString();
//                serverPort = Integer.parseInt(allComponents.editTextServerPort.getText().toString());
//                // 打印相关信息
//                Toast.makeText(getApplicationContext(),"Connect To Server \n"+serverIP+" : "+String.valueOf(serverPort),Toast.LENGTH_SHORT).show();
//                allComponents.textViewTime.setText(String.valueOf(System.currentTimeMillis()));  // 更新时间
//                // TODO 连接服务器
//            }
//        });
//        // Start按钮的点击功能
//        allComponents.buttonStart.setOnClickListener(new View.OnClickListener()
//        {
//            @Override
//            public void onClick(View view)
//            {
//                sendSensorDataFlag = true;
//            }
//        });
//
//        // Stop按钮的点击功能
//        allComponents.buttonStart.setOnClickListener(new View.OnClickListener()
//        {
//            @Override
//            public void onClick(View view)
//            {
//                sendSensorDataFlag = false;
//            }
//        });
//
//
//    }
//
//    @Override
//    protected void onResume()
//    {
//        super.onResume();
//        // 注册传感器Listener
//        // 传感器监听
//        //magneticFieldSensorEventListener = new MyPhoneSensorsEventListener(this.phoneSensorsData, this.allComponents);
//        //accelerometerSensorEventListener = new MyPhoneSensorsEventListener(this.phoneSensorsData, this.allComponents);
//        //gyroscopeSensorEventListener = new MyPhoneSensorsEventListener(this.phoneSensorsData, this.allComponents);
//        Toast.makeText(getApplicationContext(),"Register Sensors.",Toast.LENGTH_SHORT).show();
//        //sensorManager.registerListener(this.magneticFieldSensorEventListener, this.magneticFieldSensor, SensorManager.SENSOR_DELAY_UI);
//        //sensorManager.registerListener(this.accelerometerSensorEventListener, this.accelerometerSensor, SensorManager.SENSOR_DELAY_UI);
//        //sensorManager.registerListener(this.gyroscopeSensorEventListener, this.gyroscopeSensor, SensorManager.SENSOR_DELAY_UI);
//    }
//
//    @Override
//    protected void onPause()
//    {
//        super.onPause();
//        // 注销
//        Toast.makeText(getApplicationContext(),"Unregister Sensors.",Toast.LENGTH_SHORT).show();
//        //sensorManager.unregisterListener(accelerometerSensorEventListener);
//        //sensorManager.unregisterListener(magneticFieldSensorEventListener);
//        //sensorManager.unregisterListener(gyroscopeSensorEventListener);
//    }
//
//
//}
//
//
//class AllComponents
//{
//    public Button buttonConnect, buttonStart, buttonStop;
//    public TextView textViewAX,textViewAY,textViewAZ,textViewRX,textViewRY,textViewRZ,textViewTime;
//    public EditText editTextServerIP, editTextServerPort;
//}
//
//
////class MyPhoneSensorsEventListener implements SensorEventListener
////{
////    PhoneSensorsData phoneSensorsData;
////    AllComponents allComponents;
////
////    // 构造函数
////    public MyPhoneSensorsEventListener(PhoneSensorsData outerPhoneSensorsData, AllComponents outerAllComponents)
////    {
////        this.phoneSensorsData = outerPhoneSensorsData;
////        this.allComponents = outerAllComponents;
////    }
////
////    // 弧度制转角度制
////    public float[] convertAnglesFromRadiusToDegree()
////    {
////        // 都按照原来的顺序处理
////        float[] angles = new float[3];
////        angles[0] = (float) (phoneSensorsData.rotationAnglesValue[0] / Math.PI * 180);
////        angles[1] = (float) (phoneSensorsData.rotationAnglesValue[1] / Math.PI * 180);
////        angles[2] = (float) (phoneSensorsData.rotationAnglesValue[2] / Math.PI * 180);
////        return angles;
////    }
////
////
////
////
////    // 传感器值改变时触发
////    @Override
////    public void onSensorChanged(SensorEvent sensorEvent)
////    {
////        this.phoneSensorsData.currentTime = System.currentTimeMillis();
////        if (sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
////        {
////            this.phoneSensorsData.accelerometerSensorValue = sensorEvent.values;
////        } else if (sensorEvent.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
////        {
////            this.phoneSensorsData.magneticFieldSensorValue = sensorEvent.values;
////        } else if (sensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE)
////        {
////            this.phoneSensorsData.gyroscopeSensorValue = sensorEvent.values;
////        }
////
////        SensorManager.getRotationMatrix(this.phoneSensorsData.rotationMatrixValue,
////                null, this.phoneSensorsData.accelerometerSensorValue, this.phoneSensorsData.magneticFieldSensorValue);
////        SensorManager.getOrientation(this.phoneSensorsData.rotationMatrixValue, this.phoneSensorsData.rotationAnglesValue);
////
////        this.phoneSensorsData.rotationAnglesValueDegree = this.convertAnglesFromRadiusToDegree();
////
////        this.allComponents.textViewAX.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[0]));
////        this.allComponents.textViewAY.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[1]));
////        this.allComponents.textViewAZ.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[2]));
////
////        this.allComponents.textViewRX.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[0]));
////        this.allComponents.textViewRY.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[1]));
////        this.allComponents.textViewRZ.setText(String.valueOf(this.phoneSensorsData.accelerometerSensorValue[2]));
////    }
////
////    // 传感器精度改变时触发
////    @Override
////    public void onAccuracyChanged(Sensor sensor, int i)
////    {
////
////    }
////}
//
//
//class PhoneSensorsData
//{
//    // currentTime
//    public long currentTime = 0L;
//    // 传感器数据
//    public float[] rotationMatrixValue = new float[9];
//    public float[] rotationAnglesValue = new float[3];  // 角度传感器数据(弧度)
//    public float[] rotationAnglesValueDegree = new float[3];  // 角度传感器数据(角度)
//    public float[] accelerometerSensorValue = new float[3];  // 加速度传感器数据
//    public float[] magneticFieldSensorValue = new float[3];  // 地磁传感器数据
//    public float[] gyroscopeSensorValue = new float[3];  // 陀螺仪传感器数据
//
//    //public String packSensorsDataToJson()
//    //{
//    //    return "";
//    //}
//    //
//    //public static void main(String[] args)
//    //{
//    //
//    //}
//}
