#include <Arduino.h>
#include <OneButton.h>
#include <IRremote.h>

#include "main.h"
#include "fan.h"
#include "lights.h"
#include "remote.h"

Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);
Remote remote(REMOTE_PIN);

PCInfo info;
MainMode main_mode = msmLights;

uint8_t remote_index = 0;
bool remote_buttons;
bool is_remote_buttons_pressed;

uint8_t bright = BRIGHT;

unsigned long timer_info;
unsigned long timer_ir;

// парсинг
char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;



// TODO: Добавить константы 
// TODO: Добавить пропуск первого значения от пульта
// TODO: Добавить режим подсветки с выбором одного цвета



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
  String str = String("F:") + info.info.fan_mode + ',' + info.info.fan_cpu_step_temp + ',' + info.info.fan_gpu_step_temp
             + String(";L:") + info.info.lights_mode + ',' + info.info.lights_bright + ';';
  // Отправляем данные вентилятора
  Serial.print(str);
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



void setup(){
  Serial.begin(9600);
  
}



void loop() {
  leds.update(info.info.cpu_temp, info.info.gpu_temp);
  fan.update(info.info.cpu_temp, info.info.gpu_temp);
  remote.update();

  if (millis() - timer_info > 2000) {
    parse(&info);
    if (is_remote_buttons_pressed) { // Если кнопка пульта была нажата
      //sendData(info); // Отправляем данные в порт
      is_remote_buttons_pressed = false; 

    }
    timer_info = millis();
  }

  if (remote.isButtonPressed()) {
    is_remote_buttons_pressed = true;
  }


  fan.setMode(info.info.fan_mode);
  fan.setStepTemp(info.info.fan_cpu_step_temp, info.info.fan_gpu_step_temp);

  leds.setMode(info.info.lights_mode);
  leds.setBrightness(info.info.lights_bright);
}