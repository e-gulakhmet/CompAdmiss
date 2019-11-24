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
    fmOff,
    fmOn,
    fmAuto
} FanMode;         

typedef struct {
    int speed;
    int min_temp;
    int max_temp;
} DefValue; 

const DefValue def_cpu_value[] = { // Значения по умолчанию
                            {0,0,65},
                            {255,66,100},
                            };

const DefValue def_gpu_value[] = { // Значения по умолчанию
                            {0,0,45},
                            {255,46,150},
                            };                           

class Fan {
    public:
        Fan(uint8_t fan_pin);

        void update(uint8_t cpu_temp, uint8_t gpu_temp);
        void on();
        void autoMode();
        void off();
        void nextMode();
        void prevMode();
        void setSpeed();
        String getMode();
        int getSpeed() {return fan_speed_;}
    
    private:
        uint8_t fan_pin_;

        uint8_t fan_speed_;  // Скорость вентилятора 
        uint8_t cpu_temp_;   // Температура процессора
        uint8_t gpu_temp_;   // Температура видеокарты

        unsigned long fan_timer_;

        FanMode fan_mode_;
};



#endif // _FAN_H_