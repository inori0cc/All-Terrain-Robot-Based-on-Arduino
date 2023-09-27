
/////////////MPU6050引脚接线//////////////
//MPU6050丝印标识----------ArduinoUNO主板引脚
//        GND-----------------GND
//        VCC-----------------5V
//        SCL-----------------A5
//        SDA-----------------A4
//MPU6050丝印标识----------ArduinoUNO主板引脚

#include <KalmanFilter.h>    //卡尔曼滤波
#include <MsTimer2.h>        //定时中断
#include "I2Cdev.h"        
#include "MPU6050_6Axis_MotionApps20.h"//MPU6050库文件
#include "Wire.h" 

MPU6050 Mpu6050; //实例化一个 MPU6050 对象，对象名称为 Mpu6050
KalmanFilter KalFilter;//实例化一个卡尔曼滤波器对象，对象名称为 KalFilter
int16_t ax, ay, az, gx, gy, gz;  //MPU6050的三轴加速度和三轴陀螺仪数据
float Angle, Gryo_x, Gryo_y, Gryo_z;  //用于显示角度和角速度的临时变量

//***************下面是卡尔曼滤波相关变量***************//
float K1 = 0.05; // 对加速度计取值的权重
float Q_angle = 0.001, Q_gryo = 0.005;
float R_angle = 0.5 , C_0 = 1;
float dt = 0.005; //注意：dt的取值为滤波器采样时间 5ms

/**************************************************************************
函数功能：5ms中断服务函数 作者：平衡小车之家
入口参数：无
返回  值：无
**************************************************************************/
void control()
{
  sei();//全局中断开启
  Mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  //获取MPU6050陀螺仪和加速度计的数据
  KalFilter.Angletest(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gryo, R_angle, C_0, K1);//通过卡尔曼滤波获取角度
  Angle = KalFilter.angle;//卡尔曼滤波后的X轴角度
  Gryo_x= KalFilter.Gyro_x;//卡尔曼滤波后的X轴角速度
  Gryo_y= KalFilter.Gyro_y;
  Gryo_z= KalFilter.Gyro_z;
}

/**************************************************************************
函数功能：初始化 相当于STM32里面的Main函数 作者：平衡小车之家
入口参数：无
返回  值：无
**************************************************************************/
void setup() {
  Wire.begin();             //加入 IIC 总线
  Serial.begin(9600);       //开启串口，设置波特率为 9600
  delay(1500);              //延时等待初始化完成
  Mpu6050.initialize();     //初始化MPU6050
  Serial.println("检测MPU6050是否连接...");
  Serial.println(Mpu6050.testConnection() ? "MPU6050已成功连接" : "没有连接到MPU6050");
  Serial.println();
  delay(20); 
  MsTimer2::set(5, control);  //使用Timer2设置5ms定时中断
  MsTimer2::start();          //启动中断使能
}

/**************************************************************************
函数功能：主循环程序体
入口参数：无
返回  值：无
**************************************************************************/
void loop() {

    Serial.print("三轴角加速度：[ax, ay, az]= ");
    Serial.print("[");
    Serial.print(ax);//X轴角加速度
    Serial.print(", ");
    Serial.print(ay);//Y轴角加速度
    Serial.print(", ");
    Serial.print(az);//Z轴角加速度
    Serial.println("]");

    Serial.print("三轴角速度：[gx, gy, gz]= ");
    Serial.print("[");
    Serial.print(gx);//X轴角速度
    Serial.print(", ");
    Serial.print(gy);//Y轴角速度
    Serial.print(", ");
    Serial.print(gz);//Z轴角速度
    Serial.println("]");
    
    Serial.print("卡尔曼滤波后的三轴角速度：[Gryo_x, Gryo_y, Gryo_z]= ");
    Serial.print("[");
    Serial.print(Gryo_x);//卡尔曼滤波后的X轴角速度
    Serial.print(", ");
    Serial.print(Gryo_y);//卡尔曼滤波后的Y轴角速度
    Serial.print(", ");
    Serial.print(Gryo_z);//卡尔曼滤波后的Z轴角速度
    Serial.println("]");

    Serial.print("X轴最优倾角角度Angle=");
    Serial.println(Angle);//卡尔曼滤波后的X轴倾角角度

    Serial.println();
    delay(500);              //延时等待初始化完成
}
