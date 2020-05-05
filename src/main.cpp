#include <Arduino.h>
#include <OneButton.h>

#include "main.h"
#include "fan.h"
#include "lights.h"

Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);

PCInfo info;

bool is_connect = false;
unsigned long timer_info;
char inData[82]; // массив входных значений (СИМВОЛЫ)


// TODO: Добавить константы 
// TODO: Добавить режим подсветки с выбором одного цвета
// TODO: Добавить настройку скорости режимов


// Получение информации от компьютера и сохранение ее в управляющей структуре
void parse(PCInfo *info) {
  static String string_convert;
  static byte index = 0;
  if (Serial.available() > 0) {
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
      is_connect = true;
    }
  }
  else
    is_connect = false;
}



void sendData(PCInfo info) {
  String str = String("F:") + info.info.fan_mode + ',' + info.info.fan_cpu_step_temp + ',' + info.info.fan_gpu_step_temp
             + String(";L:") + info.info.lights_mode + ',' + info.info.lights_bright + ';';
  // Отправляем данные вентилятора
  Serial.print(str);
}



void setup(){
  Serial.begin(9600);
}



void loop() {
  leds.update(info.info.cpu_temp, info.info.gpu_temp);
  fan.update(info.info.cpu_temp, info.info.gpu_temp);

  if (millis() - timer_info > 2000) {
    parse(&info);
    timer_info = millis();
  }

  if (is_connect) {
    fan.setMode(static_cast<Fan::FanMode>(info.info.fan_mode));
    fan.setStepTemp(info.info.fan_cpu_step_temp, info.info.fan_gpu_step_temp);
    leds.set_on(info.info.lights_main_mode);
    leds.setMode(static_cast<Lights::LightsMode>(info.info.lights_mode));
    leds.setBrightness(info.info.lights_bright);
    leds.setSpeed(info.info.lights_speed);
  }

  else {
    fan.setMode(Fan::fmOn);
    leds.setMode(Lights::lmRainbow);
  }
}