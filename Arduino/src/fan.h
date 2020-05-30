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

class Fan {
    public:
        Fan(uint8_t fan_pin);

        typedef enum { // Перечисление всех режимов
            fmOff,
            fmOn,
            fmAuto
        } FanMode;  

        void update(uint8_t cpu_temp, uint8_t gpu_temp);
        void on();
        void autoMode();
        void off();
        void setMode(FanMode mode) {fan_mode_ = mode;};
        void nextMode();
        void prevMode();
        String getModeName();
        void setStepTemp(uint8_t cpu_temp, uint8_t gpu_temp);
        int getSpeed() {return fan_speed_;}
        
    
    private:
        uint8_t fan_pin_;

        uint8_t fan_speed_;  // Скорость вентилятора 
        uint8_t cpu_temp_;   // Температура процессора
        uint8_t gpu_temp_;   // Температура видеокарты
        uint8_t cpu_step_temp_;
        uint8_t gpu_step_temp_;

        unsigned long fan_timer_;

        String get_mode_name_[3] = {"Off", "On", "Auto"};

        FanMode fan_mode_;
};



#endif // _FAN_H_