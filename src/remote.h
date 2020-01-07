#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <Arduino.h>
#include <IRremote.h>

class Remote{
    public:
        Remote(uint8_t ir_pin);

        typedef struct {
            unsigned long one;
            unsigned long two;
            unsigned long three;
            unsigned long four;
            unsigned long five;
            unsigned long six;
            unsigned long seven;
            unsigned long eight;
            unsigned long nine;
            unsigned long star;
            unsigned long zero;
            unsigned long grid;
            unsigned long up;
            unsigned long left;
            unsigned long ok;
            unsigned long right;
            unsigned long down; 
        } RemoteButt;

        typedef union {
            RemoteButt butt;
            unsigned long info[17];
        } RemoteInfo;
        
        void update();
        void setRemoteButtons(RemoteInfo *info);
        uint8_t isInfoGot(){return results_.decode_type;}

    private:
        uint8_t ir_pin_;
        uint8_t index_;

        unsigned long timer_;

        decode_results results_;

        IRrecv remote_;
};


#endif // _REMOTE_H_