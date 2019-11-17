#include <fan.h>



Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , fan_mode_(fmOff)
    {
        pinMode(fan_pin_, OUTPUT);
        fan_speed_ = 0;
    }


void Fan::tick(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    if(!is_manual_){ // Авто выбор
        // Проверяем поступают ли данные о температуре
        // Если температура равна 0, то выключаем флаг is_online_
        if(cpu_temp_ == 0 && gpu_temp_ == 0){
            is_online_ = false;
        }
        // Если температура не равно 0, то включаем флаг is_online_
        else if(cpu_temp_ != 0 && gpu_temp_ != 0){
            is_online_ = true;
        }
        
        // Если флаг не включен, то включаем вентиляторы
        if(!is_online_){
            fan_mode_ = fmOn;
        }

        // Если флаг включен, то происходит выбор режимов и т. д.
        else if(is_online_){
            if(millis() - fan_timer_ > 5000){ // Измеряем температуру, каждые n секунд.
                fan_timer_ = millis();
                // Выбираем режим
                if(cpu_temp_ > def_cpu_value[fmOff].max_temp
                || gpu_temp_ > def_gpu_value[fmOff].max_temp){
                    fan_mode_ = fmOn;
                }
                else if(cpu_temp_ < def_cpu_value[fmOn].min_temp
                        || gpu_temp_ < def_gpu_value[fmOn].min_temp){
                    fan_mode_ = fmOff;
                }
            }
        }

    }


    // Присваем дэфолтные значения выбранного режима, скорости вентилятора(fan_speed_)
    fan_speed_ = def_cpu_value[fan_mode_].speed;

    analogWrite(fan_pin_, fan_speed_);
}



void Fan::off(){
    is_manual_ = true;
    fan_mode_ = fmOff;
}



void Fan::on(){
    is_manual_ = true;
    fan_mode_ = fmOn;
}



void Fan::autoMode(){
    is_manual_ = false;
}



String Fan::getMode(){
    switch(fan_mode_){
        case fmOff: return "Off"; break;
        case fmOn: return "On"; break;
    }
}
