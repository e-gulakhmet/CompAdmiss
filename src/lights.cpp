#include <lights.h>



Lights::Lights(uint8_t data_pin, uint8_t num_leds)
    : data_pin_(data_pin)
    , num_leds_(num_leds)
    , is_on(false)
    , is_updated(false)
    , brightness_(200)
    , color_index_(0)
    , speed_(20)
    , thishue(0)
    , thissat(255)
    , mode_(emOneColor)
    , leds_(Adafruit_NeoPixel(num_leds_, data_pin_, NEO_GRB + NEO_KHZ800))
    {

    }


void Lights::begin() {
    leds_.begin();
    leds_.setBrightness(brightness_);
    leds_.clear();
    leds_.show();
}



void Lights::update() {
    if (is_on) { // Если подсветка включена
        switch (mode_) {
            
        }
    }
    else {
        if (!is_updated) {
            leds_.clear();
            leds_.show();
            is_updated = true;
        }
    }
}

void Lights::setOn(bool state) {
    if (is_on != state) {
        is_on = state;
        is_updated = false; // Говорим, что нужно обновить ленту
    }

}

void Lights::setEffect(EffectMode mode) {
    if (mode_ == mode)
        return;

    is_updated = false; 
    mode_ = mode;
}

void Lights::setBrightness(uint8_t bright) {
    if (brightness_ == bright)
        return;

    brightness_ = bright;
    is_updated = false;
    leds_.setBrightness(brightness_);
}

void Lights::setEffectColor(uint8_t index) {
    if (color_index_ == index)
        return;
    
    color_index_ = index;
    is_updated = false;
}

void Lights::setEffectSpeed(int speed) {
    if (speed_ == speed)
        return;
    
    speed_ = speed;
    timer = millis();
}



bool Lights::safeDelay(int del_time) {
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



void Lights::PulseOneColor() {              //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
    static bool bouncedirection;
    static int ibright;

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
    for (int idex = 0 ; idex < num_leds_; idex++ ) {
        leds_.setPixelColor(idex, leds_.ColorHSV(thishue, thissat, ibright));
    }
    leds_.show();
}

void Lights::rgbProp() { //-m27-RGB PROPELLER
    static int idex;

    if (safeDelay(speed_))
        return;

    idex++;
    int ghue = (thishue + 80) % 255;
    int bhue = (thishue + 160) % 255;
    int N3  = int(num_leds_ / 3);
    int N6  = int(num_leds_ / 6);
    int N12 = int(num_leds_ / 12);
    for (int i = 0; i < N3; i++ ) {
        int j0 = (idex + i + num_leds_ - N12) % num_leds_;
        int j1 = (j0 + N3) % num_leds_;
        int j2 = (j1 + N6) % num_leds_;
        leds_.setPixelColor(j0, leds_.ColorHSV(thishue, thissat, 255));
        leds_.setPixelColor(j1, leds_.ColorHSV(ghue, thissat, 255));
        leds_.setPixelColor(j2, leds_.ColorHSV(bhue, thissat, 255));
    }
    leds_.show();
}

void Lights::rainbow() {
    static uint16_t ihue = 0;

    if (safeDelay(speed_))
        return;
    
    ihue += 500;
    if (ihue > 65536) {
        ihue = 0;
    }
    for (int idex_ = 0 ; idex_ < num_leds_; idex_++ ) {
        leds_.setPixelColor(idex_, leds_.ColorHSV(ihue, thissat, 100));
    }
    leds_.show();
}

void Lights::randomColor() { //-m25-RANDOM COLOR POP
    static int idex;
    static int ihue;

    if (safeDelay(speed_))
        return;

    idex = random(0, num_leds_);
    ihue = random(0, 255);
    for (uint8_t i = 0; i < num_leds_; i++)
        leds_.setPixelColor(i, 0, 0, 0);
    leds_.setPixelColor(idex, leds_.ColorHSV(ihue, thissat, 255));
    leds_.show();
}


