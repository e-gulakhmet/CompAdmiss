#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <Arduino.h>
#include <GyverRGB.h>

/*
    Класс подсветки, сдедит за температурой и нагрузкой видеокарты и процессора.
    При запуске включается выбранный режим подсветки, затем взависимости от режима работы
    цвета становятся более яркими(холодные цвета перестают использоваться).

    Переключение режимов подсветки проиходит, при помощи команд
    изменяющих режим подсветки(Эти режимы не зависят от температуры).

    Первый и основной режим подсветки - это изменение цвета взависимости от температуры.
    Второй режим подсветки - это радуга которая при повышении температуры,
    уходит в красные тона.
    Третий режим - это измение оттенка красного цвета,
    при повышении температуры.
*/

typedef enum { // Режимы подсветки
    lmColor, // Переключение цветов
    lmRainbow, // Радуга
    lmKelvin // Температура
} LightsMode;

// typedef enum { // Режимы температуры
//     ltmCalm,
//     ltmNormal,
//     ltmPower,
//     ltmHell   
// } LightsTempMode;

// typedef struct {
//     uint8_t min_temp;
//     uint8_t max_temp;
// } DefLightsValue;

// const DefLightsValue def_light_cpu_value[] = {
//                             {0,50},
//                             {51, 70},
//                             {71,80},
//                             {81, 150},
//                             };

// const DefLightsValue def_light_gpu_value[] = {
//                             {0,40},
//                             {41, 50},
//                             {51,60},
//                             {61, 70},
//                             };

class Lights {
    public: 
        Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);

        void tick(uint8_t cpu_temp, uint8_t gpu_temp);
        void off(); // Выключение ленты
        void on(); // Включение ленты
        void Color(); // Эффект переключения цветов
        void Rainbow(); // Эффект радуги
        void Gamma(); // Эффект температуры по кельвину
        void setBrightness(uint8_t bright);
        //String getTempMode(); // Получение режим температуры
        String getMode(); // Получение режима подцветки

    private:
        uint8_t red_pin_;
        uint8_t green_pin_;
        uint8_t blue_pin_;

        uint8_t cpu_temp_;
        uint8_t gpu_temp_;
        
        bool isOn_;

        unsigned long lights_timer_;
        unsigned long rainbow_timer_;
        bool isAlarmTimer_;
        uint8_t hsvColor_;

        LightsMode lights_mode_; // Основные режимы работы подсветки
        GRGB leds_;

        //LightsTempMode lights_temp_mode_; // Температурные режимы



};


#endif
