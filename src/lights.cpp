#include <lights.h>


Lights::Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : red_pin_(red_pin)
    , green_pin_(green_pin)
    , blue_pin_(blue_pin)
    , leds_(red_pin_, green_pin_, blue_pin_)
    , lights_mode_(lmGamma)
    {
        leds_.setBrightness(50);
    }


void Lights::tick(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    // Автоматический выбор режима
    if(millis() - lights_timer_ > 5000){
        lights_timer_ = millis();
        switch(lights_temp_mode_){
            case ltmCalm: // Если в спокойном режиме
                // Если температура больше максимального  значения, то включаем средний режим
                if(cpu_temp_ >= def_light_cpu_value[ltmCalm].max_temp || gpu_temp_ >= def_light_gpu_value[ltmCalm].max_temp){ 
                    lights_temp_mode_ = ltmNormal;
                }
                break;

            case ltmNormal: // Если в среднем режиме
                // Если температура ниже минимального значения, то включаем спкойный режим
                if(cpu_temp_ <= def_light_cpu_value[ltmNormal].min_temp && gpu_temp_ <= def_light_gpu_value[ltmNormal].min_temp){
                    lights_temp_mode_ = ltmCalm;
                }
                if(cpu_temp_ >= def_light_cpu_value[ltmNormal].max_temp || gpu_temp_ >= def_light_gpu_value[ltmNormal].max_temp){
                    lights_temp_mode_ = ltmPower;
                }
                break;
            
            case ltmPower:
                if(cpu_temp_ <= def_light_cpu_value[ltmPower].min_temp && gpu_temp_ <= def_light_gpu_value[ltmPower].min_temp){
                    lights_temp_mode_ = ltmNormal;
                }
                if(cpu_temp_ >= def_light_cpu_value[ltmPower].max_temp || gpu_temp_ >= def_light_gpu_value[ltmPower].max_temp){
                    lights_temp_mode_ = ltmHell;
                }
                break;
            
            case ltmHell:
                if(cpu_temp_ <= def_light_cpu_value[ltmHell].min_temp && gpu_temp_ <= def_light_gpu_value[ltmHell].min_temp){
                    lights_temp_mode_ = ltmPower;
                }
                break;
        }
    }


    if(lmOff == lights_mode_){
        leds_.setHEX(BLACK);
    }

    else if(lmColor == lights_mode_){
        switch(lights_temp_mode_){
        case ltmCalm: leds_.fadeTo(BLUE, 3000); break;
        case ltmNormal: leds_.fadeTo(GREEN, 3000); break;
        case ltmPower: leds_.fadeTo(YELLOW, 3000); break;
        case ltmHell: leds_.fadeTo(RED, 3000); break;
        }
    }

    else if(lmRainbow == lights_mode_){
        if(millis() - rainbow_timer_ > 10){
            rainbow_timer_ = millis();
            hsvColor_++;
            if(hsvColor_ >= 255){
                hsvColor_ = 0;
            }
        }

        switch(lights_temp_mode_){
            case ltmCalm: leds_.setHSV(hsvColor_, 255, 255); break;
            case ltmNormal: leds_.setHSV(hsvColor_, 255, 255); break;
            case ltmPower: leds_.setHSV(hsvColor_, 150, 255); break;
            case ltmHell: leds_.setHSV(hsvColor_, 100, 255); break;
        }
    }

    else if(lmGamma){
        switch(lights_temp_mode_){
            case ltmCalm: leds_.setKelvin(1000); break;
            case ltmNormal: leds_.setKelvin(1500); break;
            case ltmPower: leds_.setKelvin(2000); break;
            case ltmHell: leds_.setKelvin(3000); break;
        }
    }

}