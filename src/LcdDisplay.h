#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);



void setupLcdDisp(){
    lcd.begin();                      // Инициализация дисплея  
    lcd.backlight();                 // Подключение подсветки
}