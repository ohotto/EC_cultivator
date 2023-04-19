#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
//定义OLED屏幕
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);
//串口输入
char S_input;
bool Rrunning = 0;
bool Lrunning = 0;
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
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);   //设定屏幕型号
  display.clearDisplay();                     //清屏
  display.setTextSize(2);                     //全局字号设定为2
  //首次显示文字
  d_text(0, 0, "LEFT RIGHT", 1);
  d_text(1, 1, "G1G2G3G4", 1);
}
void loop()
{
  if (Serial.available() > 0) {
    S_input = Serial.read();
  }
  if (S_input == '1'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(1, 1, "G1", 0);
  }
  if (S_input == '2'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(3, 1, "G2", 0);
  }
  if (S_input == '3'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(5, 1, "G3", 0);
  }
  if (S_input == '4'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(7, 1, "G4", 0);
  }
  if (S_input == '1'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(1, 1, "G1", 0);
  }
  if (S_input == '1'){
    d_text(1, 1, "G1G2G3G4", 1);
    d_text(1, 1, "G1", 0);
  }
    if (S_input == 'l')
  {
    if (Lrunning == 0)
    {
      d_text(0, 0, "LEFT", 0);
      Lrunning = 1;
    }
    else
    {
      d_text(0, 0, "LEFT", 1);
      Lrunning = 0;
    }
  }
  if (S_input == 'r')
  {
    if (Rrunning == 0)
    {
      d_text(5, 0, "RIGHT", 0);
      Rrunning = 1;
    }
    else
    {
      d_text(5, 0, "RIGHT", 1);
      Rrunning = 0;
    }
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
