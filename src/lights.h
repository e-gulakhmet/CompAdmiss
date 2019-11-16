#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <Arduino.h>
#include <GyverRGB.h>

/*
    Класс подсветки, сдедит за температурой и нагрузкой видеокарты и процессора.
    При запуске включается выбранный режим подсветки, затем взависимости от режима работы
    цвета становятся более яркими(холодные цвета перестают использоваться).

*/

typedef enum {
    lmOff = -1,
    lmCalm,
    lmNormal,
    lmPower,
    lmHell   
} LightsMode;

typedef struct {
    uint8_t min_temp;
    uint8_t max_temp;
} DefLightsValue;

const DefLightsValue def_light_cpu_value[] = {
                            {0,50},
                            {51, 70},
                            {71,80},
                            {81, 150},
                            };

const DefLightsValue def_light_gpu_value[] = {
                            {0,40},
                            {41, 50},
                            {51,60},
                            {61, 70},
                            };

class Lights {
    public: 
        Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);

        void tick(uint8_t cpu_temp, uint8_t gpu_temp);
        LightsMode getMode() {return lights_mode;}

    private:
        uint8_t red_pin_;
        uint8_t green_pin_;
        uint8_t blue_pin_;

        uint8_t cpu_temp_;
        uint8_t gpu_temp_;
        
        unsigned long lights_timer_;

        GRGB leds;
        LightsMode lights_mode;
};





#endif
