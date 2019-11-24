#include <Arduino.h>
#include <SoftwareSerial.h>
#include <OneButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

#include "main.h"
#include "fan.h"
#include "lights.h"

SoftwareSerial mySerial(TX_PIN, RX_PIN);
Encoder enc(ENC_A, ENC_B, BUTT_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);

PCInfo info;
Mode mode = mMain;
MainMode main_mode = msmLights;

byte charGrad[] = {
  0x1C,
  0x14,
  0x1C,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

byte charFan[] = {
  0x00,
  0x1D,
  0x05,
  0x1F,
  0x14,
  0x17,
  0x00,
  0x00
};

byte charBright[] = {
  0x00,
  0x15,
  0x0E,
  0x1B,
  0x0E,
  0x15,
  0x00,
  0x00
};

byte charTemp[] = {
  0x04,
  0x11,
  0x12,
  0x11,
  0x15,
  0x09,
  0x0A,
  0x1F
};

uint8_t bright = 50;
bool is_select = false;
bool is_curs = false;

unsigned long timer_curs;
unsigned long timer_info;
// парсинг
char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;


// TODO: Добавить константы 
// TODO: Оптимизировать функцию возращающую имена режимов
// TODO: Оптимизировать функицию установки выбранного режима



// Получение информации от компьютера и сохранение ее в управляющей структуре
void parse(PCInfo *info){
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar != 'E') {
      inData[index] = aChar;
      index++;
      inData[index] = '\0';
    } else {
      char *p = inData;
      char *str;
      index = 0;
      String value = "";
      while ((str = strtok_r(p, ";", &p)) != NULL) {
        string_convert = str;
        info->data[index] = string_convert.toInt();
        index++;
      }
      index = 0;
    }
  }
}



void showCursor(uint8_t x, uint8_t y, bool is_blink){ // Вывод курсора
  lcd.setCursor(x, y);
  if(is_blink){
    // Моргаем курсором
    if(millis() - timer_curs > 500){
      is_curs = !is_curs;
      timer_curs = millis();
    }
    lcd.print(is_curs?'>':' ');
  }
  // Не моргаем
  else{
    lcd.print(">"); 
  }
}



MainMode switchMainMode(MainMode curr, bool clockwice){ // Переключение режимов
  int n = static_cast<int>(curr);

  n += clockwice ? 1 : -1; // Если по часовой стрелке, то ставим следующий

  if( n > 2) {
    n = 2;
  }
  if( n < 0 ){
    n = 0;
  }
  
  return static_cast<MainMode>(n);
}



// Вывод информации на дисплей
void showInfo(PCInfo *info){
  // Вывод информации о подсветке
  lcd.setCursor(1,0); lcd.print(leds.getModeName()); 
  lcd.setCursor(1,1); lcd.write(2); lcd.print("=");
  lcd.print(map(bright, 0, 255, 0, 100)); lcd.print("%");

  // Вывод информации о вентиляторах
  lcd.setCursor(10,0); lcd.write(3); lcd.print("="); lcd.print(info->info.cpu_temp); lcd.write(0);
  lcd.setCursor(10,1); lcd.write(1); lcd.print(":"); lcd.print(fan.getModeName()); 

  // Настройка курсора в разных режимах
  switch(main_mode){
    case msmLights: 
      if(is_select){ // Если мы уже выбрали режим и настраиваем его
        showCursor(0,0,false);
      }
      else{ // Если мы еще не выбрали, какой режим настраивать(кнопка не была нажата).
        showCursor(0,0,true);
      }
      break;

    case msmBright:
      if(is_select){
        showCursor(0,1,false);
      }
      else{
        showCursor(0,1,true);
      }
      break;

    case msmFan:
      if(is_select){
        showCursor(9,1,false);
      }
      else{
        showCursor(9,1,true);
      }
      break;
  }
}



void showSett(){

}



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, charGrad);
  lcd.createChar(1, charFan);
  lcd.createChar(2, charBright);
  lcd.createChar(3, charTemp);

  lcd.setCursor(4,0); lcd.print("Who Man");
  lcd.setCursor(2,1); lcd.print("Technologies");
  delay(2000);
  lcd.clear();
  
  enc.setType(TYPE1);
  enc.tick();
  if(enc.isHold()){ 
    mode = mSetting;
  }
}



void loop() {
  enc.tick();
  leds.update(info.info.cpu_temp, info.info.gpu_temp);
  fan.update(info.info.cpu_temp, info.info.gpu_temp);
  if(millis() - timer_info > 100){
    parse(&info);
    timer_info = millis();
  }

  switch(mode){
    case mMain: // Если включен рабочий режим
      showInfo(&info); // Отображаем основную информацию

      if(is_select){
        if(enc.isHold()){
            is_select = false;
        }
      }

      else if ( !is_select ) {

        if ( enc.isRight() ) {
          main_mode = switchMainMode(main_mode, true);
          lcd.clear();
        }

        else if(enc.isLeft()){
          main_mode = switchMainMode(main_mode, false);
          lcd.clear();
        }

        if(enc.isRelease()){
          is_select = true;
        }
      }
    

      switch(main_mode){ // Переключение между режимами настроек
        case msmLights: // Настройка режима подсветки
          // Настройка выбранного режима
          if(is_select){
            if(enc.isLeft()){
              leds.prevMode();
              lcd.clear();
            }
            else if(enc.isRight()){
              leds.nextMode();
              lcd.clear();
            }
          }
          break;
      
        case msmBright: // Настройка яркости
          // Если выбрали режим(кнопка была нажата)
            if(is_select){
            // Настройка выбранного режима
            bright = constrain(bright, 0, 255);
            if(enc.isLeft()){
              bright-= 5;
              lcd.clear();
            }
            else if(enc.isRight()){
              bright+= 5;
              lcd.clear();
            }
            leds.setBrightness(bright);
          }
          break;

        case msmFan: // Настройка яркости
          // Если выбрали режим(кнопка была нажата)
          if(is_select){
            // Настройка выбранного режима
            if(enc.isLeft()){
              fan.prevMode();
              lcd.clear();
            }
            else if(enc.isRight()){
              fan.nextMode();
              lcd.clear();
            }
          }
          break;

        }
        break;


    case mSetting:
      showSett(); // Показываем настройки
      break;
  }  
}


