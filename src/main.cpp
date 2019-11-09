#include <Arduino.h>
#include <SoftwareSerial.h>


#include <menu.h>
#include <Encoder.h>
#include <Button.h>

SoftwareSerial mySerial(TX_PIN, RX_PIN);




void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);

  setupEnc();

  setupLcdDisp();
  createChar();

  leds.setBrightness(BRIGHT);

  printLogo();
  delay(2000);
  lcd.clear();
}



void loop() {
  parsing();
  button.tick();

  mainMenu();
}

