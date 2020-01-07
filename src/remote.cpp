#include <remote.h>

Remote::Remote(uint8_t ir_pin)
    : ir_pin_(ir_pin)
    , remote_(ir_pin_)
    {
        pinMode(ir_pin_, INPUT);
    }



void Remote::update() {
    // Получаем данные от пульта
    if (millis() - timer_ > 1000) {

        if (remote_.decode(&results_)) { // Если данные от пульта получены
            Serial.println(results_.value); 
            remote_.resume(); // Получаем следующее значение
        }

        timer_ = millis();
    }
}



void Remote::setRemoteButtons(RemoteInfo *info){ // Устанавливаем значения кнопкам пульта
  if (millis() - timer_ > 1000) {
    if (index_ < 17) {
      if (remote_.decode(&results_)) { // Если данные от пульта получены
        index_++;
        info->info[index_] = results_.value;
        remote_.resume();
      }
    }
    timer_ = millis();
  }
}