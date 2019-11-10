#ifndef _FAN_H_
#define _FAN_H_

#include <Arduino.h>

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

        uint8_t fan_speed; // Скорость вентилятора 
        uint8_t PC_temp[2];
        uint8_t cpu_temp; // Температура процессора
        uint8_t gpu_temp; // Температура видеокарты
        
        boolean flag_check;
        uint32_t timeoutTimer;
        char inData[82];       // массив входных значений (СИМВОЛЫ)
        int PCdata[20];        // массив численных значений показаний с компьютера
        byte index = 0;
        String string_convert;

        unsigned long fan_timer;


};




#endif



