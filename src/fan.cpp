#include <fan.h>



Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , flag_check(false)
    , f_mode(calmM)
    {
        pinMode(fan_pin_, OUTPUT);
    }



void Fan::tick(){ 
  if(!flag_check && millis() - fan_timer > 5000){ // Проверяем данные по температуре каждые n секунд
    flag_check = true;
  }

  if(flag_check){
    while (Serial.available() > 0) {
      char aChar = Serial.read();
      if (aChar != 'E'){
        inData[index] = aChar;
        index++;
        inData[index] = '\0';
      }
      else{
        char *p = inData;
        char *str;
        index = 0;
        String value = "";
        while ((str = strtok_r(p, ";", &p)) != NULL) {
          string_convert = str;
          PC_temp[index] = string_convert.toInt();
          index++;
        }
        index = 0;

      }
    }
    fan_timer = millis(); 
    flag_check = false;
    cpu_temp = PC_temp[0];
    gpu_temp = PC_temp[1];
    Serial.println(cpu_temp);
  }

  if(flag_online){ // Если флаг работы вентилятора активен
    analogWrite(fan_pin_, fan_speed); // Вентилятор работает
  }
  else if(!flag_online){ // Если флаг работы вентилятора не активен
    analogWrite(fan_pin_, 0); // Вентилятор не работает
  }


  // Автоматической переключение между режимами
  // Проверяем, если температура высокая, то увеличиваем скорость
  if(cpu_temp > def_value[calmM].max_temp){
    fan_speed = def_value[normalM].speed;
  }
  if(cpu_temp < def_value[calmM].max_temp){
    fan_speed = def_value[calmM].speed;
  }
  





}