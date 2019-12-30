#include <Arduino.h>
#include <OneButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "fan.h"
#include "lights.h"

Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);

PCInfo info;
MainMode main_mode = msmLights;

uint8_t bright = BRIGHT;
bool is_select = false;
bool is_curs = false;

unsigned long timer_curs;
unsigned long timer_info;
// парсинг
char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;

// TODO: Добавить константы 
// TODO: Сделать функцию отправления данных в порт


// Получение информации от компьютера и сохранение ее в управляющей структуре
void parse(PCInfo *info) {
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar != 'E') {
      inData[index] = aChar;
      index++;
      inData[index] = '\0';
    } 
    else {
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



void sendData(PCInfo info) {
  char data[4];
  String str;
  // Отправляем данные вентилятора
  Serial.write("F:");
  for(int i = 2; i <= 3; i++) {
    str = String(info.data[i]);
    str.toCharArray(data,4);
    Serial.write(data); Serial.write(";");
  }

  // Отправляем данные подсветки
  Serial.write("L:");
  for(int i = 4; i <= 5; i++) {
    str = String(info.data[i]);
    str.toCharArray(data,4);
    Serial.write(data); Serial.write(";");
  }
}



MainMode switchMainMode(MainMode curr, bool clockwice) { // Переключение режимов
  int n = static_cast<int>(curr);

  n += clockwice ? 1 : -1; // Если по часовой стрелке, то ставим следующий

  if ( n > 2) {
    n = 2;
  }
  if ( n < 0 ) {
    n = 0;
  }
  return static_cast<MainMode>(n);
}



void setup() {
  Serial.begin(9600);
}



void loop() {
  leds.update(info.info.cpu_temp, info.info.gpu_temp);
  fan.update(info.info.cpu_temp, info.info.gpu_temp);
  if (millis() - timer_info > 2000) {
    parse(&info);
    sendData(info);
    timer_info = millis();
  }

}


