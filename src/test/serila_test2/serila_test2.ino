//串口输入
char S_input;

void setup()
{
  //设置波特率
  Serial.begin(57600);
}
void loop()
{
  if (Serial.available() > 0) {
    S_input = Serial.read();
    
  }
  Serial.print(S_input);
}
