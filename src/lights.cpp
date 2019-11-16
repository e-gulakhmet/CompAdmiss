#include <lights.h>


Lights::Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : red_pin_(red_pin)
    , green_pin_(green_pin)
    , blue_pin_(blue_pin)
    , leds(red_pin_, green_pin_, blue_pin_)
    {
        leds.setBrightness(50);
    }


void Lights::tick(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    // Автоматический выбор режима
    if(millis() - lights_timer_ > 5000){
        lights_timer_ = millis();
        switch(lights_mode){
            case lmOff: break;

            case lmCalm: // Если в спокойном режиме
                // Если температура больше максимального  значения, то включаем средний режим
                if(cpu_temp_ >= def_light_cpu_value[lmCalm].max_temp || gpu_temp_ >= def_light_gpu_value[lmCalm].max_temp){ 
                    lights_mode = lmNormal;
                }
                break;

            case lmNormal: // Если в среднем режиме
                // Если температура ниже минимального значения, то включаем спкойный режим
                if(cpu_temp_ <= def_light_cpu_value[lmNormal].min_temp && gpu_temp_ <= def_light_gpu_value[lmNormal].min_temp){
                    lights_mode = lmCalm;
                }
                if(cpu_temp_ >= def_light_cpu_value[lmNormal].max_temp || gpu_temp_ >= def_light_gpu_value[lmNormal].max_temp){
                    lights_mode = lmPower;
                }
                break;
            
            case lmPower:
                if(cpu_temp_ <= def_light_cpu_value[lmPower].min_temp && gpu_temp_ <= def_light_gpu_value[lmPower].min_temp){
                    lights_mode = lmNormal;
                }
                if(cpu_temp_ >= def_light_cpu_value[lmPower].max_temp || gpu_temp_ >= def_light_gpu_value[lmPower].max_temp){
                    lights_mode = lmHell;
                }
                break;
            
            case lmHell:
                if(cpu_temp_ <= def_light_cpu_value[lmHell].min_temp && gpu_temp_ <= def_light_gpu_value[lmHell].min_temp){
                    lights_mode = lmPower;
                }
                break;
        }
    }


    switch(lights_mode){
        case lmOff: leds.setHEX(BLACK); break;
        case lmCalm: leds.setHEX(BLUE); break;
        case lmNormal: leds.setHEX(GREEN); break;
        case lmPower: leds.setHEX(YELLOW); break;
        case lmHell: leds.setHEX(RED); break;
    }

}