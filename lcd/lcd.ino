#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    lcd.setRGB(colorR, colorG, colorB);

    lcd.print("The lcd  ");

    delay(1000);
}

void loop() {

    lcd.setCursor(0, 1);
    lcd.setCursor(1, 1);
    lcd.print("is working");


    delay(100);
}