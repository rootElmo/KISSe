#include <Arduino.h>
#include "encoder.h"
#include "pins.h"

// Move these ints to encoder.h??
// Research this. (variables in .h vs. variables in .cpp)

// these states could be put into a single byte
// then read with some bitmasking magic.
// Research, try out. Might be alot of work

// Bxy000000 (x = 1 for CW incr; x = 0 for CCW incr)
int8_t incr_amnt = 0;
// B00xyy000 (x = encoder direction, yy enc_amount)
int8_t enc_amnt = 0;
// B00000x00 (a_state true or false (HIGH or LOW))
int8_t a_state;
// B000000x0 (a_last_state true or false (HIGH or LOW))
int8_t a_last_state; 
// B0000000x (Still room for encoder button state??)

                         // (https://www.mouser.fi/datasheet/2/54/PEC11R-777457.pdf)
void encoder::init() { // Pins defined at pins.h                   
    pinMode(8, INPUT); // enca
    pinMode(7, INPUT); // encb
    Serial.println(enc_amnt);
    a_last_state = 1; // Encoder detent sits at ENC_A = HIGH
}

// Base of code copied from (https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/)

int encoder::poll() {
    // Couldn't this be done without the last_state variable?
    // Leave code as is for now, change later, possibly
    // put all variables to a single byte
    /*
    if (digitalRead(ENC_A) != a_state) {
        if (digitalRead(ENC_B) != a_state) {
            enc_amnt code here ......
        }
    }
    */
    a_state = digitalRead(8);
    if (a_state != a_last_state) {
        if (digitalRead(7) != a_state) {
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