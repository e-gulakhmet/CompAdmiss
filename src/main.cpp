#include <Arduino.h>
#include <SoftwareSerial.h>
#include <GyverRGB.h>

#include <main.h>
#include <Encoder.h>
#include <LcdDisplay.h>
#include <parsing.h>
#include <Button.h>

SoftwareSerial mySerial(TX_PIN, RX_PIN);
GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);  // куда подключены цвета (R, G, B)

int main_mode;



void calmMode(){ // Спокойный режим
  showInfo("Easy", PCdata[0], 0, 100); // Информация
}



void mainMenu(){ // Главное меню
  switch(main_mode){
    case 0: calmMode(); break; 
  }
}




void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  setupEnc();

  setupLcdDisp();
  createChar();

  leds.setBrightness(100);

  printLogo();
  delay(2000);
  lcd.clear();
}



void loop() {
  parsing();
  button.tick();

  mainMenu();

}

