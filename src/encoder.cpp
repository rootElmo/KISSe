#include <Arduino.h>
#include "encoder.h"
#include "pins.h"

// Move these ints to encoder.h??
// Research this. (variables in .h vs. variables in .cpp)

int8_t incr_amnt = 0;
int8_t enc_amnt = 0;
int8_t a_state;
int8_t a_last_state = 1; // Encoder detent sits at ENC_A = HIGH
                         // (https://www.mouser.fi/datasheet/2/54/PEC11R-777457.pdf)
void encoder::init() { // Pins defined at pins.h                   
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);
    Serial.println(enc_amnt);
}

// Base of code copied from (https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/)

int encoder::poll() {
    a_state = digitalRead(ENC_A);
    if (a_state != a_last_state) {
        if (digitalRead(ENC_B) != a_state) {
            enc_amnt ++;
        } else {
            enc_amnt --;
        }
    }
    a_last_state = a_state;
    
    if (enc_amnt >= 2) { // increment
        incr_amnt = 1;
        enc_amnt = 0;
    } else if (enc_amnt <= -2) { // decrement
        incr_amnt = -1;
        enc_amnt = 0;
    } else {
        incr_amnt = 0;
    }
    return incr_amnt;   // Return +1, 0 or -1
}