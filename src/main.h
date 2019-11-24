#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

const uint8_t GREEN_PIN = 5; // Зеленый пин ленты
const uint8_t RED_PIN = 6;   // Зеленый пин ленты
const uint8_t BLUE_PIN = 9;  // Зеленый пин ленты

const uint8_t FAN_PIN = 10;  // Пин вентилятора

const uint8_t TX_PIN = 11;
const uint8_t RX_PIN = 12;

const uint8_t ENC_A = 2;     // пин энкодера
const uint8_t ENC_B = 3;     // пин энкодера
const uint8_t ENC_TYPE = 1;  // тип энкодера, 0 или 1
const uint8_t BUTT_PIN = 7;  // пин кнопки

const uint16_t TIMEOUT = 5000; // Время после которого перестаем подключаться к компьютеру
const uint8_t BRIGHT = 100; // Стандартная яркость

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
  uint8_t mb_temp;
  uint8_t hdd_temp;
  uint8_t cpu_load;
  uint8_t gpu_load;
  uint8_t ram_use;
  uint8_t gpu_mem_use;
  uint8_t fan_max;
  uint8_t fan_min;
  uint8_t temp_max;
  uint8_t temp_min;
  uint8_t fan_manual;
  uint8_t color_manual;
  uint8_t fan_ctrl;
  uint8_t color_ctrl; 
  uint8_t bright_ctrl;
} OHMInfo;

typedef union {
  OHMInfo info;
  byte data[16];
} PCInfo;

#endif // _MAIN_H_