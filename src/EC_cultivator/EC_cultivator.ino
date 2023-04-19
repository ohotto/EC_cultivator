/*
 * v1.0
 * Copyright (C) 2022-2023 OttoLi
 * License: MIT (see LICENSE file for details)
 */
#include <TaskScheduler.h>
// 4个输入继电器引脚
#define gear_A 8 //换挡电机正极继电器
#define gear_B 9 //换挡负极继电器
#define K1 4
#define K2 5
#define K3 6
#define K4 7
// 4个输入继电器引脚状态
boolean kk1 = LOW;
boolean kk2 = LOW;
boolean kk3 = LOW;
boolean kk4 = LOW;
// 4个输入继电器引脚上一个状态
boolean kk1_last = kk1;
boolean kk2_last = kk2;
boolean kk3_last = kk3;
boolean kk4_last = kk4;
//回调函数次数判断
bool i12 = 0;
bool i23 = 0;
bool i34 = 0;
bool i13 = 0;
bool i24 = 0;
bool i14 = 0;
bool i43 = 0;
bool i32 = 0;
bool i42 = 0;
bool i21 = 0;
bool i31 = 0;
bool i41 = 0;
//声明回调函数
void task_12();
void task_23();
void task_34();
void task_13();
void task_24();
void task_14();
void task_43();
void task_32();
void task_42();
void task_21();
void task_31();
void task_41();
//声明计划任务
Task t12(469, TASK_FOREVER, &task_12);
Task t23(317, TASK_FOREVER, &task_23);
Task t34(683, TASK_FOREVER, &task_34);
Task t13(786, TASK_FOREVER, &task_13);
Task t24(1000, TASK_FOREVER, &task_24);
Task t14(1469, TASK_FOREVER, &task_14);
Task t43(683, TASK_FOREVER, &task_43);
Task t32(317, TASK_FOREVER, &task_32);
Task t42(1000, TASK_FOREVER, &task_42);
Task t21(500, TASK_FOREVER, &task_21);
Task t31(815, TASK_FOREVER, &task_31);
Task t41(1500, TASK_FOREVER, &task_41);
//调度器声明
Scheduler Sch;
//位置flag
int flag = 1;
bool running = 0;

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

void setup()
{
  Serial.begin(57600); //设置波特率
  // 4个控制继电器pinMode设置为INPUT
  pinMode(K1, INPUT);
  pinMode(K2, INPUT);
  pinMode(K3, INPUT);
  pinMode(K4, INPUT);
  //调度器初始化
  Sch.init();
  //添加任务
  Sch.addTask(t12);
  Sch.addTask(t23);
  Sch.addTask(t34);
  Sch.addTask(t13);
  Sch.addTask(t24);
  Sch.addTask(t14);
  Sch.addTask(t43);
  Sch.addTask(t32);
  Sch.addTask(t42);
  Sch.addTask(t21);
  Sch.addTask(t31);
  Sch.addTask(t41);
  pinMode(gear_A, OUTPUT);
  pinMode(gear_B, OUTPUT);
  ForwardR();
  //获取4个输入继电器引脚状态
  kk1 = digitalRead(K1);
  kk2 = digitalRead(K2);
  kk3 = digitalRead(K3);
  kk4 = digitalRead(K4);
  kk1_last = kk1;
  kk2_last = kk2;
  kk3_last = kk3;
  kk4_last = kk4;
}

void loop()
{
  Sch.execute();
  //delay(50);
  //获取4个输入继电器引脚状态
  kk1 = digitalRead(K1);
  kk2 = digitalRead(K2);
  kk3 = digitalRead(K3);
  kk4 = digitalRead(K4);
  //判断是否改变状态
  if (kk1 != kk1_last)
  {
    Serial.print("Key A\n");
    kk1_last = kk1;
    if (running == 0)
    {
      switch (flag)
      {
      case 1:
        break;
      case 2:
        t21.enable();
        break;
      case 3:
        t31.enable();
        break;
      case 4:
        t41.enable();
        break;
      }
      flag = 1;
    }
  }
  if (kk2 != kk2_last)
  {
    Serial.print("Key B\n");
    kk2_last = kk2;
    if (running == 0)
    {
      switch (flag)
      {
      case 1:
        t12.enable();
        break;
      case 2:
        break;
      case 3:
        t32.enable();
        break;
      case 4:
        t42.enable();
        break;
      }
      flag = 2;
    }
  }
  if (kk3 != kk3_last)
  {
    Serial.print("Key C\n");
    kk3_last = kk3;
    if (running == 0)
    {
      switch (flag)
      {
      case 1:
        t13.enable();
        break;
      case 2:
        t23.enable();
        break;
      case 3:
        break;
      case 4:
        t43.enable();
        break;
      }
      flag = 3;
    }
  }
  if (kk4 != kk4_last)
  {
    Serial.print("Key D\n");
    kk4_last = kk4;
    if (running == 0)
    {
      switch (flag)
      {
      case 1:
        t14.enable();
        break;
      case 2:
        t24.enable();
        break;
      case 3:
        t34.enable();
        break;
      case 4:
        break;
      }
      flag = 4;
    }
  }
}

//回调函数
void task_12()
{
  if (i12 == 0)
  {
    Serial.print("A -> B Start\n");
    ReverseR();
    running = 1;
    i12 = 1;
  }
  else
  {
    Serial.print("A -> B End\n");
    Pause();
    running = 0;
    i12 = 0;
    t12.disable();
  }
}
void task_23()
{
  if (i23 == 0)
  {
    Serial.print("B -> C Start\n");
    ReverseR();
    running = 1;
    i23 = 1;
  }
  else
  {
    Serial.print("B -> C End\n");
    Pause();
    running = 0;
    i23 = 0;
    t23.disable();
  }
}
void task_34()
{
  if (i34 == 0)
  {
    Serial.print("C -> D Start\n");
    ReverseR();
    running = 1;
    i34 = 1;
  }
  else
  {
    Serial.print("C -> D End\n");
    Pause();
    running = 0;
    i34 = 0;
    t34.disable();
  }
}
void task_13()
{
  if (i13 == 0)
  {
    Serial.print("A -> C Start\n");
    ReverseR();
    running = 1;
    i13 = 1;
  }
  else
  {
    Serial.print("A -> C End\n");
    Pause();
    running = 0;
    i13 = 0;
    t13.disable();
  }
}
void task_24()
{
  if (i24 == 0)
  {
    Serial.print("B -> D Start\n");
    ReverseR();
    running = 1;
    i24 = 1;
  }
  else
  {
    Serial.print("B -> D End\n");
    Pause();
    running = 0;
    i24 = 0;
    t24.disable();
  }
}
void task_14()
{
  if (i14 == 0)
  {
    Serial.print("A -> D Start\n");
    ReverseR();
    running = 1;
    i14 = 1;
  }
  else
  {
    Serial.print("A -> D End\n");
    Pause();
    running = 0;
    i14 = 0;
    t14.disable();
  }
}
void task_43()
{
  if (i43 == 0)
  {
    Serial.print("D -> C Start\n");
    ForwardR();
    running = 1;
    i43 = 1;
  }
  else
  {
    Serial.print("D -> C End\n");
    Pause();
    running = 0;
    i43 = 0;
    t43.disable();
  }
}
void task_32()
{
  if (i32 == 0)
  {
    Serial.print("C -> B Start\n");
    ForwardR();
    running = 1;
    i32 = 1;
  }
  else
  {
    Serial.print("C -> B End\n");
    Pause();
    running = 0;
    i32 = 0;
    t32.disable();
  }
}
void task_42()
{
  if (i42 == 0)
  {
    Serial.print("D -> B Start\n");
    ForwardR();
    running = 1;
    i42 = 1;
  }
  else
  {
    Serial.print("D -> B End\n");
    Pause();
    running = 0;
    i42 = 0;
    t42.disable();
  }
}
void task_21()
{
  if (i21 == 0)
  {
    Serial.print("B -> A Start\n");
    ForwardR();
    running = 1;
    i21 = 1;
  }
  else
  {
    Serial.print("B -> A End\n");
    Pause();
    running = 0;
    i21 = 0;
    t21.disable();
  }
}
void task_31()
{
  if (i31 == 0)
  {
    Serial.print("C -> A Start\n");
    ForwardR();
    running = 1;
    i31 = 1;
  }
  else
  {
    Serial.print("C -> A End\n");
    Pause();
    running = 0;
    i31 = 0;
    t31.disable();
  }
}
void task_41()
{
  if (i41 == 0)
  {
    Serial.print("D -> A Start\n");
    ForwardR();
    running = 1;
    i41 = 1;
  }
  else
  {
    Serial.print("D -> A End\n");
    Pause();
    running = 0;
    i41 = 0;
    t41.disable();
  }
}
