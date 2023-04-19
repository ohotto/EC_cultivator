void setup()
{
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    Serial.begin(9600); // 设置串口波特率为9600
}

int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;
int val7 = 0;
int val8 = 0;
int val9 = 0;
int val10 = 0;
int val11 = 0;
int val12 = 0;

void loop()
{

    if (val1 != digitalRead(2))
    { // 按键1
        if (val1 == 0)
        {
            Serial.write('A');
            val1 = 1;
        }
        else
        {
            Serial.write('B');
            val1 = 0;
        }
    }

    if (val2 != digitalRead(3))
    { // 按键2
        if (val2 == 0)
        {
            Serial.write('C');
            val2 = 1;
        }
        else
        {
            Serial.write('D');
            val2 = 0;
        }
    }

    if (val3 != digitalRead(4))
    { // 按键3
        if (val3 == 0)
        {
            Serial.write('E');
            val3 = 1;
        }
        else
        {
            Serial.write('F');
            val3 = 0;
        }
    }

    if (val4 != digitalRead(5))
    { // 按键4
        if (val4 == 0)
        {
            Serial.write('G');
            val4 = 1;
        }
        else
        {
            Serial.write('H');
            val4 = 0;
        }
    }

    if (val5 != digitalRead(6))
    { // 按键5
        if (val5 == 0)
        {
            Serial.write('I');
            val5 = 1;
        }
        else
        {
            Serial.write('J');
            val5 = 0;
        }
    }

    if (val6 != digitalRead(7))
    { // 按键6
        if (val6 == 0)
        {
            Serial.write('K');
            val6 = 1;
        }
        else
        {
            Serial.write('L');
            val6 = 0;
        }
    }

    if (val7 != digitalRead(8))
    { // 按键7
        if (val7 == 0)
        {
            Serial.write('M');
            val7 = 1;
        }
        else
        {
            Serial.write('N');
            val7 = 0;
        }
    }

    if (val8 != digitalRead(9))
    { // 按键8
        if (val8 == 0)
        {
            Serial.write('O');
            val8 = 1;
        }
        else
        {
            Serial.write('P');
            val8 = 0;
        }
    }

    if (val9 != digitalRead(10))
    { // 按键9
        if (val9 == 0)
        {
            Serial.write('Q');
            val9 = 1;
        }
        else
        {
            Serial.write('R');
            val9 = 0;
        }
    }

    if (val10 != digitalRead(11))
    { // 按键10
        if (val10 == 0)
        {
            Serial.write('S');
            val10 = 1;
        }
        else
        {
            Serial.write('T');
            val10 = 0;
        }
    }

    if (val11 != digitalRead(12))
    { // 按键11
        if (val11 == 0)
        {
            Serial.write('U');
            val11 = 1;
        }
        else
        {
            Serial.write('V');
            val11 = 0;
        }
    }

    if (val12 != digitalRead(13))
    { // 按键12
        if (val12 == 0)
        {
            Serial.write('W');
            val12 = 1;
        }
        else
        {
            Serial.write('X');
            val12 = 0;
        }
    }
    // Serial.write('A'); // 向串口发送字符'A'
    // delay(1000); // 等待1秒钟
}
