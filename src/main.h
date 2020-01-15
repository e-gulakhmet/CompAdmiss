#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

const uint8_t GREEN_PIN = 10; // Зеленый пин ленты
const uint8_t RED_PIN = 9;   // Зеленый пин ленты
const uint8_t BLUE_PIN = 6;  // Зеленый пин ленты

const uint8_t FAN_PIN = 11;  // Пин вентилятора

const uint8_t TX_PIN = 11;
const uint8_t RX_PIN = 12;

const uint8_t BRIGHT = 200; // Стандартная яркость

const uint8_t REMOTE_PIN = A0; // Пин ик приемника

typedef enum {
  mMain,
  mSetting
} Mode;

typedef enum {
  msmLights = 0,
  msmBright,
  msmFan
} MainMode;

typedef struct {
  uint8_t cpu_temp;
  uint8_t gpu_temp;
  uint8_t fan_mode;
  uint8_t fan_cpu_step_temp;
  uint8_t fan_gpu_step_temp;
  uint8_t lights_main_mode;
  uint8_t lights_mode;
  uint8_t lights_bright;
} OHMInfo;

typedef union {
  OHMInfo info;
  byte data[6];
} PCInfo;



#endif // _MAIN_H_