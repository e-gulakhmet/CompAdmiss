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