#include <Arduino.h>

#include "main.h"
#include "fan.h"
#include "lights.h"

Fan fan(FAN_PIN_2);
Lights leds(LED_PIN, NUM_LEDS);

PCInfo info;

bool is_connect = false;
unsigned long timer_info;
uint8_t rand_val_limit;
uint8_t rand_val;
unsigned long connect_timer;

uint8_t cpu_temp = 0;
uint8_t gpu_temp = 0;

char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;


// TODO: Добавить проверку пулчаемых значений
// TODO: Закончить описание проекта
// TODO: Добавить отключения модуля если данные не приходят



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
        if (string_convert.toInt() >= 0 && string_convert.toInt() <= 255)
          info->data[index] = string_convert.toInt();
        index++;
      }
      is_connect = true;
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



void setup(){
  Serial.begin(9600);
  leds.begin();
  leds.setBrightness(BRIGHT);
}



void loop() {
  leds.update(cpu_temp, gpu_temp);
  fan.update(cpu_temp, gpu_temp);

  if (millis() - timer_info > 1000) {
    timer_info = millis();
    parse(&info);

    rand_val_limit++;

    if (is_connect) {
      if (info.info.lights_main_mode == 0 || info.info.lights_main_mode == 1) {
        cpu_temp = info.info.cpu_temp;
        gpu_temp = info.info.gpu_temp;
        fan.setMode(static_cast<Fan::FanMode>(info.info.fan_mode));
        fan.setStepTemp(info.info.fan_cpu_step_temp, info.info.fan_gpu_step_temp);
        leds.setOn(info.info.lights_main_mode);
        leds.setEffect(static_cast<Lights::EffectMode>(info.info.lights_mode));
        leds.setBrightness(info.info.lights_bright);
        leds.setEffectSpeed(info.info.lights_speed);
        leds.setEffectColor(info.info.light_color * 257);
        leds.setMaxTemp(info.info.light_cpu_max_temp, info.info.light_gpu_max_temp);
        
        // if (rand_val_limit > 3 && rand_val == info.info.rand_value) {
        //   is_connect = false;
        // }
        // if (rand_val != info.info.rand_value){
        //   rand_val = info.info.rand_value;
        //   rand_val_limit = 0;
        // }
      }
    }

    else {
      fan.setMode(Fan::fmOff);
      leds.setOn(false);
    }
  }
}