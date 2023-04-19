/*
 * v2.0
 * Copyright (C) 2022-2023 OttoLi
 * License: MIT (see LICENSE file for details)
 */
#include <TaskScheduler.h>
#define gear_A 8 //换挡电机正极继电器
#define gear_B 9 //换挡负极继电器
// 4个开关信号引脚
#define K1 4
#define K2 5
#define K3 6
#define K4 7
// 开关信号引脚状态缓存
boolean kk1 = LOW;
boolean kk2 = LOW;
boolean kk3 = LOW;
boolean kk4 = LOW;
//声明回调函数
void call_Forward();
void call_Reverse();
//声明计划任务
Task t_Forward(0, TASK_FOREVER, &call_Forward);
Task t_Reverse(0, TASK_FOREVER, &call_Reverse);
//回调函数次数判断
bool ti = 0;
//前进
void ForwardR()
{
  digitalWrite(gear_A, HIGH);
  digitalWrite(gear_B, LOW);
}
//后退
void ReverseR()
{
  digitalWrite(gear_A, LOW);
  digitalWrite(gear_B, HIGH);
}
//暂停
void Pause()
{
  digitalWrite(gear_A, LOW);
  digitalWrite(gear_B, LOW);
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

void setup()
{
  //设置波特率
  Serial.begin(57600);
  // 4个开关信号pinMode设置为INPUT
  pinMode(K1, INPUT);
  pinMode(K2, INPUT);
  pinMode(K3, INPUT);
  pinMode(K4, INPUT);
  // 2个电机引脚pinMode设置为INPUT
  pinMode(gear_A, OUTPUT);
  pinMode(gear_B, OUTPUT);
  //调度器初始化
  Sch.init();
  //添加任务
  Sch.addTask(t_Forward);
  Sch.addTask(t_Reverse);
  //确保电机在初始位置
  ForwardR();
  //获取4个输入继电器引脚状态
  kk1 = digitalRead(K1);
  kk2 = digitalRead(K2);
  kk3 = digitalRead(K3);
  kk4 = digitalRead(K4);
}

void loop()
{
  Sch.execute();
  //若按键1被按下
  if (digitalRead(K1) != kk1)
  {
    Serial.print("Press Key 1\n");   //debug
    kk1 = digitalRead(K1);           //重置kk1状态缓存
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
          break;
        case 3:
          t_Forward.setInterval(DT[2][0]);
          t_Forward.enable();
          break;
        case 4:
          t_Forward.setInterval(DT[3][0]);
          t_Forward.enable();
          break;
      }
      flag = 1; //更新当前位置
    }
  }
  //若按键2被按下
  if (digitalRead(K2) != kk2)
  {
    Serial.print("Press Key 2\n");   //debug
    kk2 = digitalRead(K2);           //重置kk2状态缓存
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][1]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          break;
        case 2:
          break;
        case 3:
          t_Forward.setInterval(DT[2][1]);
          t_Forward.enable();
          break;
        case 4:
          t_Forward.setInterval(DT[3][1]);
          t_Forward.enable();
          break;
      }
      flag = 2; //更新当前位置
    }
  }
  //若按键3被按下
  if (digitalRead(K3) != kk3)
  {
    Serial.print("Press Key 3\n");   //debug
    kk3 = digitalRead(K3);           //重置kk3状态缓存
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][2]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          break;
        case 2:
          t_Reverse.setInterval(DT[1][2]);
          t_Reverse.enable();
          break;
        case 3:
          break;
        case 4:
          t_Forward.setInterval(DT[3][2]);
          t_Forward.enable();
          break;
      }
      flag = 3; //更新当前位置
    }
  }
  //若按键4被按下
  if (digitalRead(K4) != kk4)
  {
    Serial.print("Press Key 4\n");   //debug
    kk4 = digitalRead(K4);           //重置kk4状态缓存
    //判断电机是否正在运动
    if (running == 0)
    {
      switch (flag)
      {
        case 1:
          t_Reverse.setInterval(DT[0][3]);  //设定标定时间
          t_Reverse.enable();               //开启任务
          break;
        case 2:
          t_Reverse.setInterval(DT[1][3]);
          t_Reverse.enable();
          break;
        case 3:
          t_Reverse.setInterval(DT[2][3]);
          t_Reverse.enable();
          break;
        case 4:
          break;
      }
      flag = 4; //更新当前位置
    }
  }
}

//回调函数
void call_Forward()
{
  //若首次进入回调
  if (ti == 0)
  {
    ForwardR();   //前进使能
    running = 1;  //标记运动状态
    ti = 1;       //标记回调状态
  }
  //若第2次进入回调
  else
  {
    Pause();              //暂停运动
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
    ReverseR();   //前进使能
    running = 1;  //标记运动状态
    ti = 1;       //标记回调状态
  }
  //若第2次进入回调
  else
  {
    Pause();              //暂停运动
    running = 0;          //运动状态归零
    ti = 0;               //回调状态归零
    t_Reverse.disable();  //结束任务
  }
}
