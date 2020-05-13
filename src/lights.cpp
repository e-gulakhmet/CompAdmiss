#include <lights.h>



Lights::Lights(uint8_t data_pin, uint8_t num_leds)
    : data_pin_(data_pin)
    , num_leds_(num_leds)
    , is_on_(true)
    , speed_(30)
    , color_(30000)
    , mode_(emRgbPropeller)
    , leds_(Adafruit_NeoPixel(num_leds_, data_pin_, NEO_GRB + NEO_KHZ800))
    {
        leds_.setBrightness(200);
    }


void Lights::begin() {
    leds_.begin();
    leds_.setBrightness(200);
    leds_.clear();
    leds_.show();
}



void Lights::update(uint8_t cpu_temp, uint8_t gpu_temp) {
    cpu_temp_ = cpu_temp;
    gpu_temp_ = gpu_temp;
    // Оповещение о высокой температуре
    if ((cpu_temp_ >= 90 && cpu_temp_ <= 140) || (gpu_temp_ >= 80 && gpu_temp_ <= 140)) { // Если температура больше максимальной
        static unsigned long am_timer;
        static bool is_alarm_timer;
        if (millis() - am_timer > 500) { // Моргаем красным цветом.
            is_alarm_timer = !is_alarm_timer;
            am_timer = millis();
            for (int i = 0; i < num_leds_; i++)
                leds_.setPixelColor(i, (is_alarm_timer ? leds_.Color(255, 0, 0) : leds_.Color(0, 0, 0)));
        }

        return;
    }

    if (is_on_) { // Если подсветка включена
        switch (mode_) {
            case emAdaptTemp:
                adaptTemp(cpu_temp_, gpu_temp_);
                break;
            
            case emRandomColor:
                randomColor();
                break;
            
            case emPulseOneColor:
                pulseOneColor();
                break;
            
            case emRgbPropeller:
                rgbProp();
                break;

            case emRainbow:
                rainbow();
                break;
            
            case emFlicker:
                flicker();
                break;
            
            case emRandomRed:
                randomRed();
                break;
            
            case emRadiation:
                radiation();
                break;

            case emWhiteTemps:
                whiteTemps();
                break;
            
            case emQuaqBright:
                quaqBright();
                break;
            
            case emSparkle:
                sparkle();
                break;
        }
    }
    else {
        leds_.clear();
        leds_.show();
    }
}

bool Lights::safeDelay(int del_time) {
    static unsigned long timer;
    if (millis() - timer < del_time)
        return true;

    timer = millis();
    return false;
}

int Lights::antipodal_index(int i) {
    int iN = i + num_leds_ / 2;
    if (i >= num_leds_ / 2) {
        iN = (i + num_leds_ / 2) % num_leds_;
    }
    return iN;
}



void Lights::pulseOneColor() {              //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
    static bool bouncedirection;
    static uint8_t ibright;

    if (safeDelay(speed_)) 
        return;

    if (bouncedirection == 0) {
            ibright++;
        if (ibright >= 255) {
            bouncedirection = 1;
        }
    }
    if (bouncedirection == 1) {
        ibright = ibright - 1;
        if (ibright <= 1) {
            bouncedirection = 0;
        }
    }

    leds_.fill(color_pallete_[color_index_], 0, num_leds_);
    leds_.show();
}

void Lights::rgbProp() { //-m27-RGB PROPELLER
    static uint8_t idex;

    if (safeDelay(speed_))
        return;

    idex++;

    if (idex > 255)
        idex = 0;

    uint16_t ghue = (color_ + (int)(65536 / 3)) % 65536;
    uint16_t bhue = (color_ + (int)(65536 / 2)) % 65536;
    int N3  = int(num_leds_ / 3);
    int N6  = int(num_leds_ / 6);
    int N12 = int(num_leds_ / 12);
    for (int i = 0; i < N3; i++ ) {
        int j0 = (idex + i + num_leds_ - N12) % num_leds_;
        int j1 = (j0 + N6) % num_leds_;
        int j2 = (j1 + N3) % num_leds_;
        leds_.setPixelColor(j0, leds_.ColorHSV(color_, 255, 255));
        leds_.setPixelColor(j1, leds_.ColorHSV(ghue, 255, 255));
        leds_.setPixelColor(j2, leds_.ColorHSV(bhue, 255, 255));
    }
    leds_.show();
}

void Lights::rainbow() {
    static uint32_t ihue = 0;

    if (safeDelay(speed_))
        return;

    ihue += 100;

    if (ihue >= 65536) {
        ihue = 0;
    }
    for (int idex = 0 ; idex < num_leds_; idex++ ) {
        leds_.setPixelColor(idex, leds_.ColorHSV(ihue, 255, 255));
    }
    leds_.show();
}

void Lights::randomColor() { //-m25-RANDOM COLOR POP
    static int idex;
    static uint32_t ihue;

    if (safeDelay(speed_))
        return;

    idex = random(0, num_leds_);
    ihue = random(0, 65536);
    for (uint8_t i = 0; i < num_leds_; i++)
        leds_.setPixelColor(i, 0, 0, 0);
    leds_.setPixelColor(idex, leds_.ColorHSV(ihue, 255, 255));
    leds_.show();
}

void Lights::adaptTemp(uint8_t cpu_temp, uint8_t gpu_temp) {
    static uint32_t old_cpu_temp = 0;
    static uint32_t old_gpu_temp = 0;

    uint32_t new_cpu_temp = cpu_temp * 65536 / 90;
    uint32_t new_gpu_temp = gpu_temp * 65536 / 80;


    if (safeDelay(speed_))
        return;

    
    if (new_cpu_temp >= new_gpu_temp) {
        if (old_cpu_temp != new_cpu_temp) {
            if (old_cpu_temp < new_cpu_temp)
                old_cpu_temp+=100;
            else if (old_cpu_temp > new_cpu_temp)
                old_cpu_temp-=100;
            leds_.fill(leds_.ColorHSV(old_cpu_temp, 255, 255), 0, num_leds_);
            leds_.show();
        }
    }
    else {
        if (old_gpu_temp != new_gpu_temp){
            if (old_gpu_temp < new_gpu_temp)
                old_gpu_temp+=100;
            else if (old_gpu_temp > new_gpu_temp)
                old_gpu_temp-=100;
            leds_.fill(leds_.ColorHSV(old_gpu_temp, 255, 255), 0, num_leds_);
            leds_.show();
        }
    }
}

void Lights::flicker() {
    static int random_delay = 0;

    if (safeDelay(random_delay))
        return;

    int random_bright = random(0, 255);
    random_delay = random(1, 40);
    int random_bool = random(0, random_bright);
    if (random_bool < 10) {
        for (int i = 0 ; i < num_leds_; i++ ) {
            leds_.setPixelColor(i, leds_.ColorHSV(color_, 255, random_bright));
        }
    leds_.show();
  }
}

void Lights::randomRed() {                       //QUICK 'N DIRTY RANDOMIZE TO GET CELL AUTOMATA STARTED
  int temprand;
  int red;
  for (int i = 0; i < num_leds_; i++ ) {
    temprand = random(0, 100);
    if (temprand > 50) {
      red = 255;
    }
    if (temprand <= 50) {
      red = 0;
    }
    leds_.setPixelColor(i, leds_.Color(red, 0, 0));
  }
  leds_.show();
}

void Lights::radiation() {
    static float tcount;
    static uint8_t ibright;

    if (safeDelay(speed_))
        return;

    int N3  = int(num_leds_ / 3);
    int N6  = int(num_leds_ / 6);
    int N12 = int(num_leds_ / 12);
    for (int i = 0; i < N6; i++ ) {    //-HACKY, I KNOW...
        tcount = tcount + .02;
        if (tcount > 3.14) {
        tcount = 0.0;
        }
        ibright = int(sin(tcount) * 255);
        int j0 = (i + num_leds_ - N12) % num_leds_;
        int j1 = (j0 + N3) % num_leds_;
        int j2 = (j1 + N3) % num_leds_;
        leds_.setPixelColor(j0, leds_.ColorHSV(color_, 255, ibright));
        leds_.setPixelColor(j1, leds_.ColorHSV(color_, 255, ibright));
        leds_.setPixelColor(j2, leds_.ColorHSV(color_, 255, ibright));
    }
    leds_.show();
}

void Lights::whiteTemps() {
    if (safeDelay(speed_))
        return;

    int red;
    int green;
    int blue;
    int N9 = int(num_leds_ / 9);
    for (int i = 0; i < num_leds_; i++ ) {
        if (i >= 0 && i < N9) {
        red = 255;  //-CANDLE - 1900
        green = 147;
        blue = 41;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));
        }
        if (i >= N9 && i < N9 * 2) {
        red = 255;  //-40W TUNG - 2600
        green = 197;
        blue = 143;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));
        }
        if (i >= N9 * 2 && i < N9 * 3) {
        red = 255;  //-100W TUNG - 2850
        green = 214;
        blue = 170;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));
        }
        if (i >= N9 * 3 && i < N9 * 4) {
        red = 255;  //-HALOGEN - 3200
        green = 241;
        blue = 224;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));        
        }
        if (i >= N9 * 4 && i < N9 * 5) {
        red = 255;  //-CARBON ARC - 5200
        green = 250;
        blue = 244;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));   
        }
        if (i >= N9 * 5 && i < N9 * 6) {
        red = 255;  //-HIGH NOON SUN - 5400
        green = 255;
        blue = 251;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));   
        }
        if (i >= N9 * 6 && i < N9 * 7) {
        red = 255;  //-DIRECT SUN - 6000
        green = 255;
        blue = 255;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));   
        }
        if (i >= N9 * 7 && i < N9 * 8) {
        red = 201;  //-OVERCAST SKY - 7000
        green = 226;
        blue = 255;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));   
        }
        if (i >= N9 * 8 && i < num_leds_) {
        red = 64;  //-CLEAR BLUE SKY - 20000
        green = 156;
        blue = 255;
        leds_.setPixelColor(i, leds_.Color(red, green, blue));   
        }
        leds_.show();
    }

}

void Lights::quaqBright() {
    static uint8_t ibright;

    if (safeDelay(speed_))
        return;

    int ax;
    for (int x = 0; x < num_leds_; x++ ) {
        if (x <= num_leds_ / 2) {
        ax = x;
        }
        else if (x > num_leds_ / 2) {
        ax = num_leds_ - x;
        }
        int a = 1; int b = 1; int c = 0;
        int iquad = -(ax * ax * a) + (ax * b) + c; //-ax2+bx+c
        int hquad = -(num_leds_ / 2 * num_leds_ / 2 * a) + (num_leds_ / 2 * b) + c;
        ibright = int((float(iquad) / float(hquad)) * 255);
        
        leds_.setPixelColor(x, leds_.ColorHSV(color_, 255, ibright));
    }
    leds_.show();
}

void Lights::sparkle() {
    uint8_t pixel = random(num_leds_);
    leds_.setPixelColor(pixel, color_pallete_[color_index_]);
    leds_.show();
    if (safeDelay(speed_))
        return;
    leds_.setPixelColor(pixel, 0, 0, 0);
}
