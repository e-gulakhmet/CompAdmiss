#include <fan.h>



Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , fan_speed_(0)
    , fan_mode_(fmAuto)
    {
        pinMode(fan_pin_, OUTPUT);
    }


void Fan::update(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    switch(fan_mode_){
        case fmOff:
            fan_speed_ = 0;
            break;

        case fmOn: 
            fan_speed_ = 255; 
            break;

        case fmAuto:
            // Проверяем поступают ли данные о температуре
            // Если температура равна 0, то выключаем флаг is_online_
            if(cpu_temp_ == 0 && gpu_temp_ == 0){
                // Если флаг не включен, то включаем вентиляторы
                fan_speed_ = 255;
                break;
            }

            // Если температура не равно 0, то измеряем температуру       
            if(millis() - fan_timer_ > 5000){ // Измеряем температуру, каждые 5 секунд.
                fan_timer_ = millis();
                // Выбираем режим
                if(cpu_temp_ > 65 || gpu_temp_ > 45){
                    fan_speed_ = 255;
                }
                else if(cpu_temp_ < 66 || gpu_temp_ < 46){
                    fan_speed_ = 0;
                }
            }
            break;
        }

    // Присваем дэфолтные значения выбранного режима, скорости вентилятора(fan_speed_)
    analogWrite(fan_pin_, fan_speed_);
}



void Fan::off(){
    fan_mode_ = fmOff;
}



void Fan::on(){
    fan_mode_ = fmOn;
}



void Fan::autoMode(){
    fan_mode_ = fmAuto;
}



String Fan::getModeName(){
    return get_mode_name_[fan_mode_];
}



void Fan::nextMode(){
    switch(fan_mode_){
        case fmOff: fan_mode_ = fmOn; break;
        case fmOn: fan_mode_ = fmAuto; break;
        case fmAuto: break;
    }
}



void Fan::prevMode(){
    switch(fan_mode_){
        case fmOff: break;
        case fmOn: fan_mode_ = fmOff; break;
        case fmAuto: fan_mode_ = fmOn; break;
    }
}