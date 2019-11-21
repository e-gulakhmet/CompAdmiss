#include <Arduino.h>
#include <SoftwareSerial.h>
#include <OneButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

#include <main.h>
#include <fan.h>
#include <lights.h>

SoftwareSerial mySerial(TX_PIN, RX_PIN);
Encoder enc(ENC_A, ENC_B, BUTT_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Fan fan(FAN_PIN);
Lights leds(RED_PIN, GREEN_PIN, BLUE_PIN);

typedef enum {
  msmLights = 0,
  msmBright,
  msmFan
} MainSettingMode;

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
MainSettingMode main_sett_mode = msmLights;

byte charGrad[] = {
  0x1C,
  0x14,
  0x1C,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

byte charFan[] = {
  0x00,
  0x1D,
  0x05,
  0x1F,
  0x14,
  0x17,
  0x00,
  0x00
};

byte charBright[] = {
  0x00,
  0x15,
  0x0E,
  0x1B,
  0x0E,
  0x15,
  0x00,
  0x00
};

uint8_t bright = 50;
bool isSelect;
bool isCurs;

unsigned long timer_curs;
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
  enc.tick();
  leds.tick(info->info.cpu_temp, info->info.gpu_temp);
  fan.tick(info->info.cpu_temp, info->info.gpu_temp);

  // Вывод информации о подсветке
  lcd.setCursor(0,0); lcd.print(leds.getMode()); 
  lcd.setCursor(0,1); lcd.write(2); lcd.print("="); lcd.print(bright);

  // Вывод информации о вентиляторах
  lcd.setCursor(10,0); lcd.write(1); lcd.print(":"); lcd.print(fan.getMode()); 

  // TOASK: Как сделать main_sett_mode = main_sett_mode + 1;

  if(!isSelect){
    if(enc.isRight()){
      switch(main_sett_mode){
        case msmLights: main_sett_mode = msmBright; break;
        case msmBright: main_sett_mode = msmFan; break;
        case msmFan: break;
      }
    }
    if(enc.isLeft()){
      switch(main_sett_mode){
        case msmLights: break;
        case msmBright: main_sett_mode = msmLights; break;
        case msmFan: main_sett_mode = msmBright; break;
      }
    }

  if(enc.isRelease()){
      isSelect = true;
    }
  }

  else if(isSelect){
    if(enc.isHold()){
        isSelect = false;
      }
  }

switch(main_sett_mode){ // Переключение между режимами настроек
  case msmLights: 
    // Если мы еще не выбрали, какой режим настраивать(кнопка не была нажата).
    if(!isSelect){
      // Маргаем курсором
      lcd.setCursor(sizeof(leds.getMode()) - 1,0);
      if(millis() - timer_curs > 500){
        isCurs = !isCurs;
        timer_curs = millis();
      }
      if(isCurs){
        lcd.print("_"); 
      }
      else{
        lcd.print(" "); 
      }
    }

    // Если выбрали режим(кнопка была нажата)
    else{
      lcd.setCursor(sizeof(leds.getMode()) - 1,0);
      lcd.print("_"); 
      // Настройка выбранного режима
      if(enc.isLeft()){
        leds.prevMode();
      }
      else if(enc.isRight()){
        leds.nextMode();
      }
    }
    break;
  
    case msmBright: // Настройка яркости
      break;

    case msmFan: // Настройка яркости
      break;

  }
}




void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, charGrad);
  lcd.createChar(1, charFan);
  lcd.createChar(2, charBright);

  lcd.setCursor(4,0); lcd.print("Who Man");
  lcd.setCursor(2,1); lcd.print("Technologies");
  delay(2000);
  lcd.clear();
  
  enc.setType(TYPE1);
}



void loop() {
  if(millis() - timer_info > 5000){
    parse(&info);
    timer_info = millis();
  }
  


  show_info(&info);
}


