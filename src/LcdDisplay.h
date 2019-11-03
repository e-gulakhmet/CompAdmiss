#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);



byte char_fan[8] = {B00000, B11101, B00101, B11111, B10100, B10111, B00000, B00000};    // Картинка вентилятора
byte char_grad[8] = {B11100, B10100, B11100, B00000, B00000, B00000, B00000, B00000};   // Картинка градуса
byte char_temp[8] = {B00100, B10100, B10101, B01001, B01011, B10010, B01000, B11111};   // Картинка температуры
byte char_bright[8] = {B00000, B10101, B01110, B11111, B01110, B10101, B00000, B00000}; // Картинка яркости



void setupLcdDisp(){
    lcd.begin();        // Инициализация дисплея  
    lcd.backlight();    // Подключение подсветки
}



void createChar(){
    lcd.createChar(0, char_fan);
    lcd.createChar(1, char_temp);
    lcd.createChar(2, char_grad);
    lcd.createChar(3, char_bright);
}



void printLogo(){ // Вывод логотипа
    lcd.setCursor(4,0);
    lcd.print("WhoMan");
    lcd.setCursor(2,1);
    lcd.print("Technologies");
}



void showInfo(String mode_name, int temp, int fan_speed, int bright){ // Вывод основной информации о режимах
    lcd.setCursor(0,0);
    lcd.print("Mode:"); lcd.print(mode_name);
    lcd.setCursor(11,0);
    lcd.write(1); lcd.print("="); lcd.print(temp); lcd.write(2);
    lcd.setCursor(2,1);
    lcd.write(0); lcd.print("="); lcd.print(fan_speed); lcd.print("%");
    lcd.setCursor(8,1);
    lcd.write(3); lcd.print("="); lcd.print(bright);
}