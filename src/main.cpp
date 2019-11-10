#include <Arduino.h>
#include <SoftwareSerial.h>
#include <GyverRGB.h>
#include <OneButton.h>

#include <main.h>
#include <Encoder.h>
#include <Button.h>
#include <parsing.h>
#include <display.h>

SoftwareSerial mySerial(TX_PIN, RX_PIN);
GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);  // куда подключены цвета (R, G, B)



void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);

  setupEnc();
  
  leds.setBrightness(BRIGHT);

  delay(2000);
}



void loop() {
  parsing();
  button.tick();
}

