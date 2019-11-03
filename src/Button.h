#include <Arduino.h>
#include <OneButton.h>


OneButton button(BUTT_PIN, true);



void click1(){

}



void setupButton(){
    button.attachClick(click1);
}
