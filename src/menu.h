#include <Arduino.h>
#include <GyverRGB.h>

#include <main.h>
#include <LcdDisplay.h>
#include <parsing.h>

GRGB leds(RED_PIN, GREEN_PIN, BLUE_PIN);  // куда подключены цвета (R, G, B)

typedef enum{
  calmM = 0, 
  normalM,
  powerM,
  hellM
} MainMode;

MainMode m_mode = calmM;



void calmMode(){ // Спокойный режим
  showInfo("Easy", PCdata[0], 0, 100); // Информация
}



void normMode(){ // Средний режим
  showInfo("Norm", PCdata[0], 0, 100); // Информация
}



void powerMode(){ // Производительный режим
  showInfo("Power", PCdata[0], 0, 100); // Информация
}



void hellMode(){ // Адский режим
  showInfo("Hell", PCdata[0], 0, 100); // Информация
}



void mainMenu(){ // Главное меню
  switch(m_mode){
    case calmM: calmMode(); break;
    case normalM: normMode(); break;
    case powerM: powerMode(); break;
    case hellM: hellMode(); break;
  }
}


