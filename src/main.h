#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

const uint8_t LED_PIN = 3; // Пин ленты
const uint8_t NUM_LEDS = 77;   // Количество диодов

const uint8_t FAN_PIN_1 = 5;  // Пин вентилятора
const uint8_t FAN_PIN_2 = 6;  // Пин вентилятора

const uint8_t BRIGHT = 200; // Стандартная яркость

typedef struct {
  uint8_t cpu_temp;
  uint8_t gpu_temp;
  uint8_t fan_mode;
  uint8_t fan_cpu_step_temp;
  uint8_t fan_gpu_step_temp;
  uint8_t lights_main_mode;
  uint8_t lights_mode;
  uint8_t lights_bright;
  uint8_t lights_speed;
} OHMInfo;

typedef union {
  OHMInfo info;
  byte data[10];
} PCInfo;



#endif // _MAIN_H_