#include <Arduino.h>
const int GREEN_PIN = 5; // Зеленый пин ленты
const int RED_PIN = 6;   // Зеленый пин ленты
const int BLUE_PIN = 9;  // Зеленый пин ленты

const int FAN_PIN = 9;  // Пин вентилятора

const int TX_PIN = 11;
const int RX_PIN = 12;

const int ENC_A = 2;     // пин энкодера
const int ENC_B = 3;     // пин энкодера
const int ENC_TYPE = 1;  // тип энкодера, 0 или 1
const int BUTT_PIN = 7;  // пин кнопки

const int TIMEOUT = 5000; // Время после которого перестаем подключаться к компьютеру
const int BRIGHT = 100; // Стандартная яркость