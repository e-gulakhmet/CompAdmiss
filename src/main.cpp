#include <Arduino.h>
#include <SoftwareSerial.h>
#include <GyverRGB.h>
#include <OneButton.h>
#include <Wire.h>
#include <GyverEncoder.h>

#include <main.h>
#include <Encoder.h>
#include <parsing.h>


SoftwareSerial mySerial(TX_PIN, RX_PIN);
GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);
Encoder enc(ENC_A, ENC_B, BUTT_PIN, 1);




// парсинг
boolean onlineFlag;
uint32_t timeoutTimer;
char inData[82];       // массив входных значений (СИМВОЛЫ)
int PCdata[20];        // массив численных значений показаний с компьютера
byte index = 0;
String string_convert;




void parsing() {
  if (onlineFlag && millis() - timeoutTimer > TIMEOUT) {
    onlineFlag = false;
  }
  
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar != 'E') {
      inData[index] = aChar;
      index++;
      inData[index] = '\0';
    } else {
      char *p = inData;
      char *str;
      index = 0;
      String value = "";
      while ((str = strtok_r(p, ";", &p)) != NULL) {
        string_convert = str;
        PCdata[index] = string_convert.toInt();
        index++;
      }
      index = 0;
      timeoutTimer = millis();
      onlineFlag = true;
    }
  }
}



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);

  
  leds.setBrightness(BRIGHT);

  delay(2000);
}



void loop() {
  parsing();
  enc.tick();

  if (enc.isRight()) Serial.println("Right");         // если был поворот
  if (enc.isLeft()) Serial.println("Left");
  
  if (enc.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc.isLeftH()) Serial.println("Left holded");
}


