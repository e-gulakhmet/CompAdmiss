#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/*
    Класс подсветки, следит за температурой и нагрузкой видеокарты и процессора.
    При запуске включается выбранный режим подсветки, затем в зависимости от режима работы
    цвета становятся более яркими(холодные цвета перестают использоваться).

    Цвета ленты напрямую зависят от температуры, так как используется "map"

    Первый и основной режим подсветки - это изменение цвета в зависимости от температуры.
    Второй режим подсветки - это радуга которая при повышении температуры,
    уходит в белые тона.
    Третий режим - это измение оттенка красного цвета,
    при повышении температуры.
*/

class Lights {
    public: 
        Lights(uint8_t data_pin, uint8_t num_leds);

        typedef enum {
            emAdaptTemp, // Один цвет
            emRandomColor,
            emPulseOneColor,
            emRgbPropeller,
            emRainbow
        } EffectMode;
        
        void begin(); // Инициализация
        void update(uint8_t cpu_temp, uint8_t gpu_temp); // Функция в которой происходит обработка всех действий
        void setOn(bool state) {is_on_ = state;}; // Включение или выключение подсветки
        void setEffect(EffectMode mode) {mode_ = mode;}; // Установка режима подсветки
        void setBrightness(uint8_t bright) {leds_.setBrightness(bright);}; // Установка яркости
        void setEffectSpeed(int speed) {speed_ = speed;}; // Установка скорости эффектов
        void setEffectColor(uint16_t color) {color_index_ = color;};
        void pulseOneColor();
        void rgbProp();
        void rainbow();
        void randomColor();
        void adaptTemp();

    private:
        uint8_t data_pin_;
        uint8_t num_leds_;

        uint8_t cpu_temp_;
        uint8_t gpu_temp_;

        bool is_on_;
        int speed_;
        uint32_t color_;
        uint8_t color_index_;

        const uint32_t color_pallete_[13] = {
            leds_.Color(255, 0, 0),
            leds_.Color(255, 0, 255),
            leds_.Color(128, 0, 128),
            leds_.Color(86, 0, 111),
            leds_.Color(2, 0, 111),
            leds_.Color(0, 100, 255),
            leds_.Color(0, 128, 128),
            leds_.Color(0, 128, 0),
            leds_.Color(0, 255, 15),
            leds_.Color(251, 255, 0),
            leds_.Color(255, 173, 0),
            leds_.Color(255, 87, 0),
            leds_.Color(0, 243, 255)
        };

        EffectMode mode_;
        Adafruit_NeoPixel leds_;

        bool safeDelay(int del_time);
        int antipodal_index(int i);
};


#endif // _LIGHTS_H_
