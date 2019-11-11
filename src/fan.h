#ifndef _FAN_H_
#define _FAN_H_

/*
    Класс вентилятора, сам решает какой режим сейчас использовать,
    но мы можем сами выбрать режим вентиляторов работы вентилятора.
    При этом когда режим выбрали мы, автоматический выбор отключается(Класс вентилятора сам не меняет режимы)
    Но если температура привысила допустимые значения на дисплее будет
    выводится предупреждение.
*/



#include <Arduino.h>

typedef enum { // Перечисление всех режимов
    fmOff = -1,
    fmCalm,
    fmNormal,
    fmPower,
    fmHell
} FanMode;         

typedef struct {
    int speed;
    int min_temp;
    int max_temp;
} DefValue; 

const DefValue def_value[] = { // Значения по умолчанию
                            {0,0,55},
                            {100,56,70},
                            {200,71,80},
                            {255,81,150},
                            };

class Fan {
    public:
        Fan(uint8_t fan_pin);

        void tick(uint8_t c_temp, uint8_t g_temp);
        void on();
        void off();
        void set_speed();
    
    private:
        uint8_t fan_pin_;

        uint8_t fan_speed_;  // Скорость вентилятора 
        uint8_t c_temp_;   // Температура процессора
        uint8_t g_temp_;   // Температура видеокарты

        FanMode f_mode_;
};


#endif



