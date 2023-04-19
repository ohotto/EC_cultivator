/*
 * v2.3
 * Copyright (C) 2022-2023 OttoLi
 * License: MIT (see LICENSE file for details)
 */

#include <TaskScheduler.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

//#define leftm_A 3   //左刹车电机正极继电器
//#define leftm_B 4   //左刹车电机负极继电器
//#define rightm_A 5  //右刹车电机正极继电器
//#define rightm_B 6  //右刹车电机负极继电器
//#define gear_A 7    //换挡电机正极继电器
//#define gear_B 8    //换挡电机负极继电器
//定义左刹车正负、右刹车正负、换挡电机正负引脚
int pin[6] = {3, 4, 5, 6, 7, 8};
//串口输入
char S_input;
//声明回调函数
void call_Forward();
void call_Reverse();
//声明计划任务
Task t_Forward(0, TASK_FOREVER, &call_Forward);
Task t_Reverse(0, TASK_FOREVER, &call_Reverse);
//回调函数次数判断
bool ti = 0;
//前进
void ForwardR(int pinA, int pinB)
{
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
}
//后退
void ReverseR(int pinA, int pinB)
{
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);
}
//暂停
void Pause(int pinA, int pinB)
{
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
}
//标定时间全局变量
int DT[4][4] = 
{
  {0,469,786,1469},
  {500,0,317,1000},
  {815,317,0,683},
  {1500,1000,683,0}
};
//调度器声明
Scheduler Sch;
//当前位置flag
int flag = 1;
//运动中flag
bool running = 0;
bool Rrunning = 0;
bool Lrunning = 0;

//定义OLED屏幕
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

//2行20字坐标位置
uint16_t loc_x[] = {5, 17, 29, 41, 53, 65, 77, 89, 101, 113};
uint16_t loc_y[] = {1, 17};

//OLED字符串打印函数
//输入参数：首个字符的x、y坐标（从0开始），字符串，显示颜色（0白底黑字、1黑底白字）
//注意：字号必须为2
void d_text(int x, int y, char text[], bool white);

void setup()
{
  //设置波特率
  Serial.begin(57600);
  // 6个电机引脚pinMode设置为INPUT
  for (size_t i = 0; i < 6; i++)
  {
    pinMode(pin[i], OUTPUT);
  }
  //调度器初始化
  Sch.init();
  //添加任务
  Sch.addTask(t_Forward);
  Sch.addTask(t_Reverse);
  //确保电机在初始位置
  ForwardR(pin[0], pin[1]);
  ForwardR(pin[2], pin[3]);
  ForwardR(pin[4], pin[5]);
  //初始化OLED屏幕
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);   //设定屏幕型号
  display.clearDisplay();                     //清屏
  display.setTextSize(2);                     //全局字号设定为2
  //首次显示文字
  d_text(0, 0, "LEFT RIGHT", 1);
  d_text(1, 1, "G1G2G3G4", 1);
  d_text(1, 1, "G1", 0);
}

void loop()
{
  Sch.execute();
  //串口控制函数段
  if (Serial.available() > 0) {
    S_input = Serial.read();
  }
  if (S_input == '1')
  {
    S_input = 0;
    Serial.print("Press Key 1\n");   //debug
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          break;
        case 2:
          t_Forward.setInterval(DT[1][0]);  //设定标定时间
          t_Forward.enable();               //开启任务
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(1, 1, "G1", 0);
          break;
        case 3:
          t_Forward.setInterval(DT[2][0]);
          t_Forward.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(1, 1, "G1", 0);
          break;
        case 4:
          t_Forward.setInterval(DT[3][0]);
          t_Forward.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(1, 1, "G1", 0);
          break;
      }
      flag = 1; //更新当前位置
    }
  }
  //若按键2被按下
  if (S_input == '2')
  {
    S_input = 0;
    Serial.print("Press Key 2\n");   //debug
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][1]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(3, 1, "G2", 0);
          break;
        case 2:
          break;
        case 3:
          t_Forward.setInterval(DT[2][1]);
          t_Forward.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(3, 1, "G2", 0);
          break;
        case 4:
          t_Forward.setInterval(DT[3][1]);
          t_Forward.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(3, 1, "G2", 0);
          break;
      }
      flag = 2; //更新当前位置
    }
  }
  //若按键3被按下
  if (S_input == '3')
  {
    S_input = 0;
    Serial.print("Press Key 3\n");   //debug
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][2]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(5, 1, "G3", 0);
          break;
        case 2:
          t_Reverse.setInterval(DT[1][2]);
          t_Reverse.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(5, 1, "G3", 0);
          break;
        case 3:
          break;
        case 4:
          t_Forward.setInterval(DT[3][2]);
          t_Forward.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(5, 1, "G3", 0);
          break;
      }
      flag = 3; //更新当前位置
    }
  }
  //若按键4被按下
  if (S_input == '4')
  {
    S_input = 0;
    Serial.print("Press Key 4\n");   //debug
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][3]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(7, 1, "G4", 0);
          break;
        case 2:
          t_Reverse.setInterval(DT[1][3]);
          t_Reverse.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(7, 1, "G4", 0);
          break;
        case 3:
          t_Reverse.setInterval(DT[2][3]);
          t_Reverse.enable();
          d_text(1, 1, "G1G2G3G4", 1);
          d_text(7, 1, "G4", 0);
          break;
        case 4:
          break;
      }
      flag = 4; //更新当前位置
    }
  }
  //若左刹车控制按钮按下
  if (S_input == 'l')
  {
    S_input = 0;
    if (Lrunning == 0)
    {
      ReverseR(pin[0], pin[1]);
      d_text(0, 0, "LEFT", 0);
      Lrunning = 1;
    }
    else
    {
      ForwardR(pin[0], pin[1]);
      d_text(0, 0, "LEFT", 1);
      Lrunning = 0;
    }
  }
  //若右刹车控制按钮按下
  if (S_input == 'r')
  {
    S_input = 0;
    if (Rrunning == 0)
    {
      ReverseR(pin[2], pin[3]);
      d_text(5, 0, "RIGHT", 0);
      Rrunning = 1;
    }
    else
    {
      ForwardR(pin[2], pin[3]);
      d_text(5, 0, "RIGHT", 1);
      Rrunning = 0;
    }
  }

}

//回调函数
void call_Forward()
{
  //若首次进入回调
  if (ti == 0)
  {
    ForwardR(pin[4], pin[5]);   //前进使能
    running = 1;  //标记运动状态
    ti = 1;       //标记回调状态
  }
  //若第2次进入回调
  else
  {
    Pause(pin[4], pin[5]);              //暂停运动
    running = 0;          //运动状态归零
    ti = 0;               //回调状态归零
    t_Forward.disable();  //结束任务
  }
}
void call_Reverse()
{
  //若首次进入回调
  if (ti == 0)
  {
    ReverseR(pin[4], pin[5]);   //前进使能
    running = 1;  //标记运动状态
    ti = 1;       //标记回调状态
  }
  //若第2次进入回调
  else
  {
    Pause(pin[4], pin[5]);              //暂停运动
    running = 0;          //运动状态归零
    ti = 0;               //回调状态归零
    t_Reverse.disable();  //结束任务
  }
}

//OLED字符串打印函数
//输入参数：首个字符的x、y坐标（从0开始），字符串，显示颜色（0白底黑字、1黑底白字）
//注意：字号必须为2
void d_text(int x, int y, char text[], bool white)
{
  if(white == 1)
  {
    display.fillRect(loc_x[x] - 1, loc_y[y] - 1, 12 * strlen(text), 16, BLACK);
    display.setTextColor(WHITE);
    display.setCursor(loc_x[x], loc_y[y]);
    display.println(text);
    display.display();
  }
  else
  {
    display.fillRect(loc_x[x] - 1, loc_y[y] - 1, 12 * strlen(text), 16, WHITE);
    display.setTextColor(BLACK);
    display.setCursor(loc_x[x], loc_y[y]);
    display.println(text);
    display.display();
  }
}
