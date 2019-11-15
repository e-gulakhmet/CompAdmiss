#include <fan.h>



Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , fan_mode_(fmOff)
    {
        pinMode(fan_pin_, OUTPUT);
        fan_speed_ = 0;
    }


void Fan::tick(uint8_t cpu_temp, uint8_t gpu_temp){
    // Измеряем температуру, каждые n секунд.
    if(millis() - fan_timer_ > 5000){
        fan_timer_ = millis();
        cpu_temp_ = cpu_temp;
        gpu_temp_ = gpu_temp;
        // Выбираем режим
        if(cpu_temp_ > def_cpu_value[fmOff].max_temp
        || gpu_temp_ > def_gpu_value[fmOff].max_temp){
            fan_mode_ = fmPower;
        }
        else if(cpu_temp_ < def_cpu_value[fmPower].min_temp
                || gpu_temp_ < def_gpu_value[fmPower].min_temp){
            fan_mode_ = fmOff;
        }
    }

    // Присваем дэфолтные значения выбранного режима, скорости вентилятора(fan_speed_)
    fan_speed_ = def_cpu_value[fan_mode_].speed;

    analogWrite(fan_pin_, fan_speed_);
}