#include <Arduino.h>
#include <OneButton.h>
#include <IRremote.h>

#include "main.h"
#include "fan.h"
#include "lights.h"

Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);
IRrecv remote(REMOTE_PIN);

PCInfo info;
MainMode main_mode = msmLights;
RemoteInfo remote_info;

uint8_t remote_index = 0;

uint8_t bright = BRIGHT;
bool is_select = false;
bool is_curs = false;

unsigned long timer_info;
unsigned long timer_ir;

// парсинг
char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;

decode_results results;



// TODO: Добавить константы 
// TODO: Создать класс пульта




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
  String str = String("F:") + info.info.fan_mode + ',' + info.info.fan_step_temp 
             + String(";L:") + info.info.lights_mode + ',' + info.info.lights_bright + ';';
  // Отправляем данные вентилятора
  Serial.print(str);
}



void setRemoteButt(RemoteInfo *info){ // Устанавливаем значения кнопкам пульта
  if (millis() - timer_ir > 1000) {
    if (remote_index < 17) {
      if (remote.decode(&results)) { // Если данные от пульта получены
        remote_index++;
        info->info[remote_index] = results.value;
        remote.resume();
      }
    }
    timer_ir = millis();
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

  remote.enableIRIn(); // Включаем получение данных от пульта
}



void loop() {
  leds.update(info.info.cpu_temp, info.info.gpu_temp);
  fan.update(info.info.cpu_temp, info.info.gpu_temp);
  if (millis() - timer_info > 2000) {
    parse(&info);
    //sendData(info);
    timer_info = millis();
  }
  
  
  if(millis() - timer_ir > 1000){
    if (remote.decode(&results)) { // Если данные от пульта получены
      Serial.println(results.value); 
      // sendData(info); // Отправляем данные компьютеру
      remote.resume(); // Получаем следующее значение
    }
    timer_ir = millis();
  }
}