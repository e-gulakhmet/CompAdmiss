#include <Arduino.h>

volatile int encCounter;
int enc_counter_old;
volatile boolean state0, lastState, turnFlag;



void int0() {
state0 = bitRead(PIND, ENC_A);
if (state0 != lastState) {
#if (ENC_TYPE == 1)
turnFlag = !turnFlag;
if (turnFlag)
encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
#else
encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
#endif
lastState = state0;
}
}


void setupEnc(){
    attachInterrupt(0, int0, CHANGE);
}



bool encRight(){ // Если поворачивается вправо то true
    if(encCounter > enc_counter_old){
        enc_counter_old = encCounter;
        return true;
    }
    else{
        return false;
    }
}

bool encLeft(){ // Если поворачивается влево то true
    if(encCounter < enc_counter_old){
        enc_counter_old = encCounter;
        return true;
    }
    else{
        return false;
    }
}
