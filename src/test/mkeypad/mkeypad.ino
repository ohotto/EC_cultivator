#include <Keypad.h>
const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
//char keys[ROWS][COLS] = {
//    {'3', '6', '9', '#'},
//    {'2', '5', '8', '0'},
//    {'1', '4', '7', '*'}};
byte rowPins[ROWS] = {6,5,4,3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {9,8,7};    // connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    char keyyy = keypad.getKey(); // Read the key

    // Print if key pressed
    if (keyyy)
    {
        Serial.print("Key Pressed : ");
        Serial.println(keyyy);
    }
}
