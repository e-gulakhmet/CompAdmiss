#include <lights.h>

Lights::Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : red_pin_(red_pin)
    , green_pin_(green_pin)
    , blue_pin_(blue_pin)
    , isOn_(true)
    , lights_mode_(lmColor)
    , leds_(red_pin_, green_pin_, blue_pin_)
    {
        leds_.setBrightness(50);
    }


void Lights::tick(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;

    if(cpu_temp_ >= 85 || gpu_temp_ >= 70){
        if(millis() - lights_timer_ > 500){
            isAlarmTimer_ = !isAlarmTimer_;
            lights_timer_ = millis();
        }
        if(isAlarmTimer_){
            leds_.setHEX(BLACK);
        }
        else{
            leds_.setHEX(RED);
        }
    }

    else{
        if(isOn_){
            switch(lights_mode_){
                case lmColor:
                    if(cpu_temp_ > gpu_temp_){
                        hsvColor_ = map(cpu_temp_, 40, 85, 128, 255);
                    }
                    else{
                        hsvColor_ = map(gpu_temp_, 30, 85, 128, 255);
                    }
                    leds_.setHSV(hsvColor_, 255, 255);
                    break;
                
                case lmRainbow:
                    uint8_t sat; 
                    if(cpu_temp_ > gpu_temp_){
                        sat = map(cpu_temp_, 0, 85, 255, 50);
                    }
                    else{
                        sat = map(gpu_temp_, 0, 85, 255, 50);
                    }
                    if(millis() - rainbow_timer_ > 10){
                        rainbow_timer_ = millis();
                        hsvColor_++;
                        if(hsvColor_ >= 255){
                        hsvColor_ = 0;
                        }
                    }
                    leds_.setHSV(hsvColor_, sat, 255);
                    break;

                case lmKelvin:
                    if(cpu_temp_ > gpu_temp_){
                        leds_.setKelvin(cpu_temp_ * 100);
                    }
                    else{
                        leds_.setKelvin(gpu_temp_ * 100);
                    }
                    break;
            }
        }

        else if(!isOn_){
            leds_.setHEX(BLACK);
        }
    }





    // if(isOn){ // Автоматический выбор режима
    //     if(millis() - lights_timer_ > 5000){
    //         lights_timer_ = millis();
    //         switch(lights_temp_mode_){
    //             case ltmCalm: // Если в спокойном режиме
    //                 // Если температура больше максимального  значения, то включаем средний режим
    //                 if(cpu_temp_ >= def_light_cpu_value[ltmCalm].max_temp || gpu_temp_ >= def_light_gpu_value[ltmCalm].max_temp){ 
    //                     lights_temp_mode_ = ltmNormal;
    //                 }
    //                 break;

    //             case ltmNormal: // Если в среднем режиме
    //                 // Если температура ниже минимального значения, то включаем спкойный режим
    //                 if(cpu_temp_ <= def_light_cpu_value[ltmNormal].min_temp && gpu_temp_ <= def_light_gpu_value[ltmNormal].min_temp){
    //                     lights_temp_mode_ = ltmCalm;
    //                 }
    //                 if(cpu_temp_ >= def_light_cpu_value[ltmNormal].max_temp || gpu_temp_ >= def_light_gpu_value[ltmNormal].max_temp){
    //                     lights_temp_mode_ = ltmPower;
    //                 }
    //                 break;
                
    //             case ltmPower:
    //                 if(cpu_temp_ <= def_light_cpu_value[ltmPower].min_temp && gpu_temp_ <= def_light_gpu_value[ltmPower].min_temp){
    //                     lights_temp_mode_ = ltmNormal;
    //                 }
    //                 if(cpu_temp_ >= def_light_cpu_value[ltmPower].max_temp || gpu_temp_ >= def_light_gpu_value[ltmPower].max_temp){
    //                     lights_temp_mode_ = ltmHell;
    //                 }
    //                 break;
                
    //             case ltmHell:
    //                 if(cpu_temp_ <= def_light_cpu_value[ltmHell].min_temp && gpu_temp_ <= def_light_gpu_value[ltmHell].min_temp){
    //                     lights_temp_mode_ = ltmPower;
    //                 }
    //                 break;
    //         }
    //     }

        
    //     // TOASK: Через switch или if и else if

    //     if(lmColor == lights_mode_){
    //         switch(lights_temp_mode_){
    //         case ltmCalm: leds_.fadeTo(BLUE, 3000); break;
    //         case ltmNormal: leds_.fadeTo(GREEN, 3000); break;
    //         case ltmPower: leds_.fadeTo(YELLOW, 3000); break;
    //         case ltmHell: leds_.fadeTo(RED, 3000); break;
    //         }
    //     }

    //     else if(lmRainbow == lights_mode_){
    //         if(millis() - rainbow_timer_ > 10){
    //             rainbow_timer_ = millis();
    //             hsvColor_++;
    //             if(hsvColor_ >= 255){
    //                 hsvColor_ = 0;
    //             }
    //         }

    //         switch(lights_temp_mode_){
    //             case ltmCalm: leds_.setHSV(hsvColor_, 255, 255); break;
    //             case ltmNormal: leds_.setHSV(hsvColor_, 255, 255); break;
    //             case ltmPower: leds_.setHSV(hsvColor_, 150, 255); break;
    //             case ltmHell: leds_.setHSV(hsvColor_, 100, 255); break;
    //         }
    //     }

    //     else if(lmGamma){
    //         switch(lights_temp_mode_){
    //             case ltmCalm: leds_.setKelvin(1000); break;
    //             case ltmNormal: leds_.setKelvin(1500); break;
    //             case ltmPower: leds_.setKelvin(2000); break;
    //             case ltmHell: leds_.setKelvin(3000); break;
    //         }
    //     }
    // }

    // else if(!isOn){ // Если мы выключили ленту, то перестаем считать температуру
    //     leds_.setHEX(BLACK);
    // }


}



void Lights::off(){ 
    isOn_ = false;
}



void Lights::on(){ 
    isOn_ = true;
}



void Lights::setBrightness(uint8_t bright){
    leds_.setBrightness(bright);
}



String Lights::getMode(){
    switch(lights_mode_){
        case lmColor: return "Color"; break;
        case lmRainbow: return "Rainbow"; break;
        case lmKelvin: return "Kelvin"; break;
    }
}