#include <lights.h>

Lights::Lights(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : red_pin_(red_pin)
    , green_pin_(green_pin)
    , blue_pin_(blue_pin)
    , is_on_(true)
    , red_(0)
    , green_(0)
    , blue_(0)
    , speed_(50)
    , lights_mode_(lmColor)
    , leds_(red_pin_, green_pin_, blue_pin_)
    {
        leds_.setBrightness(200);
    }



void Lights::update(uint8_t cpu_temp, uint8_t gpu_temp){
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;
    static unsigned long am_timer;

    // Оповещение о высокой температуре
    if (cpu_temp_ >= 90 || gpu_temp_ >= 85){ // Если температура больше максимальной
        if (millis() - am_timer > 500){ // Моргаем красным цветом.
            is_alarm_timer_ = !is_alarm_timer_;
            am_timer = millis();
        }
        if (is_alarm_timer_){
            leds_.setHEX(BLACK);
        }
        else{
            leds_.setHEX(RED);
        }
        return;
    }

    // Если температура в рабочих значениях
    if (is_on_) { // Если подсветка включена
        switch(lights_mode_) {
            case lmColor: // Режим изменения цвета
                // Если температура процессора больше температуры видеокарты,
                // то подсветка зависит от температуры процессора
                if (cpu_temp_ > gpu_temp_) {
                    if(cpu_temp_ >= 30 && cpu_temp_ <= 60){
                        red_ = 0;
                        green_ = map(cpu_temp_, 30, 60, 255, 0);
                        blue_ = map(cpu_temp_, 30, 60, 0, 255);
                    }
                    if(cpu_temp_ >= 61 && cpu_temp_ <= 90){
                        green_ = 0;
                        blue_ = map(cpu_temp_, 61, 90, 255, 0);
                        red_ = map(cpu_temp_, 61, 90, 0, 255);
                    }
                    leds_.fadeTo(red_, green_, blue_, 1000);
                }
                // Если температура видеокарты больше температуры процессора,
                // то подсветка зависит от температуры видеокарты
                else{
                    if(gpu_temp_ >= 30 && gpu_temp_ <= 55){
                        red_ = 0;
                        green_ = map(gpu_temp_, 40, 60, 255, 0);
                        blue_ = map(gpu_temp_, 40, 60, 0, 255);
                    }
                    if(gpu_temp_ >= 56 && gpu_temp_ <= 75){
                        green_ = 0;
                        blue_ = map(gpu_temp_, 51, 85, 255, 0);
                        red_ = map(gpu_temp_, 51, 85, 0, 255);
                    }
                leds_.fadeTo(red_, green_, blue_, 1000);
                }
                break;
            
            case lmRainbow: // Режим радуги
                static unsigned long rainbow_timer;
                static uint8_t hsv_color;
                if (millis() - rainbow_timer > speed_) { 
                    rainbow_timer = millis();
                    hsv_color++;
                    if (hsv_color >= 255) {
                    hsv_color = 0;
                    }
                }
                leds_.setHSV(hsv_color, 255, 255);
                break;

            case lmKelvin: // Режим отображения цвета температуры в кельвинах
                if (cpu_temp_ > gpu_temp_) {
                    leds_.setKelvin(cpu_temp_ * 10);
                }
                else{
                    leds_.setKelvin(gpu_temp_ * 10);
                }
                break;
        }
    }
    else {
        leds_.setHEX(BLACK);
    }
}



void Lights::nextMode() {
    switch(lights_mode_) {
        case lmColor: lights_mode_ = lmRainbow; break;
        case lmRainbow: lights_mode_ = lmKelvin; break;
        case lmKelvin: break;
    }
}

void Lights::prevMode() {
    switch(lights_mode_) {
        case lmColor: break;
        case lmRainbow: lights_mode_ = lmColor; break;
        case lmKelvin: lights_mode_ = lmRainbow; break;
    }
}