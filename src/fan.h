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
    calmM,
    normalM,
    powerM,
    hellM
} FanMode;         

typedef struct {
    int speed;
    int min_temp;
    int max_temp;
} DefValue; 

const DefValue def_value[] = {
                            {0,0,0},
                            {100,0,0},
                            {200,0,0},
                            {255,0,0}
                            };




class Fan{
    public:
        Fan(uint8_t fan_pin);

        void tick();
        void On();
        void Off();
        void setSpeed();
        uint8_t getTempCPU() {return cpu_temp;}
        uint8_t getTempGPU() {return gpu_temp;}
    
    private:
        uint8_t fan_pin_;

        uint8_t fan_speed;  // Скорость вентилятора 
        uint8_t PC_temp[2];
        uint8_t cpu_temp;   // Температура процессора
        uint8_t gpu_temp;   // Температура видеокарты
        
        bool flag_check;    // Флаг проверки температуры
        bool flag_online;   // Основной флаг работы вентилятора 

        char inData[82];    // массив входных значений (СИМВОЛЫ)
        uint8_t index;
        String string_convert;

        unsigned long fan_timer;

        FanMode f_mode;


};




#endif



