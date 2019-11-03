#include <Arduino.h>
#include <SoftwareSerial.h>
#include <GyverRGB.h>

#include <main.h>
#include <Encoder.h>
#include <LcdDisplay.h>

SoftwareSerial mySerial(TX_PIN, RX_PIN);
GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);  // куда подключены цвета (R, G, B)



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  setupEnc();

  setupLcdDisp();
  leds.setBrightness(100);
}



void loop() {

  if(encRight()){
    Serial.println("enc_right");
  }
  if(encLeft()){
    Serial.println("enc_left");
  }

  leds.setHEX(LIME);
}