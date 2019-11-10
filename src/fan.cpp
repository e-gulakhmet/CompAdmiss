#include <fan.h>

Fan::Fan(uint8_t fan_pin)
    : fan_pin_(fan_pin)
    , flag_check(false)
    {
        pinMode(fan_pin_, OUTPUT);
    }



void Fan::tick(){ 
  if(!flag_check && millis() - fan_timer > 5000){
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
}