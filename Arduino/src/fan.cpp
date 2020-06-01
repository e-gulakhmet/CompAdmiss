#include <fan.h>



Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , fan_speed_(0)
    , cpu_temp_(0)
    , gpu_temp_(0)
    , cpu_step_temp_(60)
    , gpu_step_temp_(55)
    , fan_mode_(fmOff)
    {
        pinMode(fan_pin_, OUTPUT);
    }


void Fan::update(uint8_t cpu_temp, uint8_t gpu_temp) {
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    switch(fan_mode_) {
        case fmOff:
            fan_speed_ = 0;
            break;

        case fmOn: 
            fan_speed_ = 255; 
            break;

        case fmAuto:    
            if (cpu_temp_ != 0 && gpu_temp_ != 0) {
                if (millis() - fan_timer_ > 500) { // Измеряем температуру, каждые 2 секунд.
                    fan_timer_ = millis();
                    // Выбираем режим
                    if (cpu_temp_ > cpu_step_temp_ || gpu_temp_ > gpu_step_temp_)
                        fan_speed_ = 255;
                    else
                        fan_speed_ = 0;
                }
            }
            break;
    }

    analogWrite(fan_pin_, fan_speed_);
}



void Fan::off() {
    fan_mode_ = fmOff;
}



void Fan::on() {
    fan_mode_ = fmOn;
}



void Fan::autoMode() {
    fan_mode_ = fmAuto;
}



String Fan::getModeName() {
    return get_mode_name_[fan_mode_];
}



void Fan::setStepTemp(uint8_t cpu_step_temp, uint8_t gpu_step_temp) {
    cpu_step_temp_ = cpu_step_temp;
    gpu_step_temp_ = gpu_step_temp;
}