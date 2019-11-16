#include <Arduino.h>
#include <SoftwareSerial.h>
#include <GyverRGB.h>
#include <OneButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

#include <main.h>
#include <fan.h>


SoftwareSerial mySerial(TX_PIN, RX_PIN);
GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);
Encoder enc(ENC_A, ENC_B, BUTT_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Fan fan(FAN_PIN);

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

PCInfo info;

unsigned long timer_info;

// парсинг
char inData[82];       // массив входных значений (СИМВОЛЫ)
byte index = 0;
String string_convert;



void parse(PCInfo *info){
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
        info->data[index] = string_convert.toInt();
        index++;
      }
      index = 0;
    }
  }
}



void show_info(PCInfo *info){
  lcd.setCursor(0,0);
  lcd.print("Manual");
  lcd.print(fan.isManual());
  lcd.setCursor(0,1);
  lcd.print("Spd");
  lcd.print(fan.getSpeed());


  fan.tick(info->info.cpu_temp, info->info.gpu_temp);
}



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);

  lcd.begin();
  lcd.backlight();
  
  leds.setBrightness(BRIGHT);

  enc.setType(TYPE1);    // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип
}



void loop() {
  if(millis() - timer_info > 5000){
    parse(&info);
    timer_info = millis();
  }
  
  enc.tick();
  
  if (enc.isRight()) fan.on();         // если был поворот
  if (enc.isLeft()) fan.off();
  if (enc.isRightH()) fan.autoMode(); // если было удержание + поворот
  if (enc.isLeftH()) Serial.println("Left holded");

  show_info(&info);
}


