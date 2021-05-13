#include <Arduino.h>
#include "display.h"
#include <SPI.h>
#include "font.h"

int digit_display[4] = {0, 0, 0, 0}; // 4th, 3rd, 2nd, 1st display
uint16_t send_char[4] = {0, 0, 0, 0};
int cs_pins[4] = {18, 17, 16, 15};
int digit_counter = 0;
int display_num = 0;
elapsedMillis updater = 0;
int UPDATE_TIME = 10;
uint16_t driver_num = 0;
int last_pin = 0;

namespace kisse {

void display::init() {
    // LED drivers
    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    // CS for display
    pinMode(23, OUTPUT);

    SPI.begin();
    // CS for display
    digitalWrite(23, HIGH);

    // LED drivers
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    digitalWrite(18, LOW);

    set_num(0);
}

void display::test_flash() {

    for (int i = 0; i < 10; i++) {
        //uint16_t test = 0x1111;
        digitalWrite(15, LOW);
        uint8_t a = characters[i] >>  8; // shift bits for first 74HC595
        uint8_t b = characters[i] & 0xFF; // mask off bits for 2nd 74HC595
        digitalWrite(23, LOW);
        SPI.setBitOrder(LSBFIRST);

        digitalWrite(15, HIGH);
        SPI.setBitOrder(LSBFIRST);
        SPI.transfer(b); // write to last 595
        SPI.transfer(a); // write to first 595
        digitalWrite(23, HIGH);
        delay(100);
        

    }
}

// Find out what the hecking heck is going on.
// displays 1 and 3 are fine, 2 and 4 are FUBAR
// Leading zeroes are also not blanked (??) even though they get assigned
// all 0 bits (characters[10]). See what the buggery that's about

// Check connections on breadboard, could have something to do with that
// Button matrix leds were flashing like new year's eve. Could be just the
// floating CS pin? Hardcoded said pin HIGH for testing. (fix later, really dumb-dumb move)

// Check the functions, if they are ran in correct order. This is some primetime late night coding
// have to get fresh eyes on this another time

void display::set_num(int num) {
    if (display_num == num) {
        return;
    }
    display_num = num;
    digit_display[3] = abs(display_num) % 10;
    digit_display[2] = abs(display_num) / 10 % 10;
    digit_display[1] = abs(display_num) / 100 % 10;
    digit_display[0] = abs(display_num) / 1000 % 10;

    Serial.print(abs(display_num) / 1000 % 10);
    Serial.print(abs(display_num) / 100 % 10);
    Serial.print(abs(display_num) / 10 % 10);
    Serial.println(abs(display_num) % 10);
    
    if (abs(display_num) < 1000) {
        digit_display[0] = 10;
    } else if (abs(display_num) < 100) {
        digit_display[1] = 10;
    } else if (abs(display_num) < 10) {
        digit_display[2] = 10;
    }
    send_char[0] = characters[digit_display[3]];
    send_char[1] = characters[digit_display[2]];
    send_char[2] = characters[digit_display[1]];
    send_char[3] = characters[digit_display[0]];
}

void display::next_digit() {
    last_pin = digit_counter;
    digit_counter++;
    if (digit_counter > 3) {
        digit_counter = 0;
    }
}

void display::driver() {
    if (updater < UPDATE_TIME) {
        return;
    }
    
    driver_num++;
    if (driver_num > 9999) {
        driver_num = 0;
    }
    set_num(driver_num);
    updater = 0;
}

void display::update() {
    digitalWrite(cs_pins[last_pin], LOW);
    uint8_t a = (send_char[3 - digit_counter]) >> 8;
    uint8_t b = (send_char[3 - digit_counter]) & 0xFF;
    digitalWrite(23, LOW);
    SPI.setBitOrder(LSBFIRST);
    
    SPI.transfer(b);
    SPI.transfer(a);
    digitalWrite(cs_pins[digit_counter], HIGH);
    digitalWrite(23, HIGH);
    
    
    /*
    digitalWrite(cs_pins[digit_counter], LOW);
    digitalWrite(23, LOW);
    SPI.setBitOrder(LSBFIRST);
    uint8_t a = (characters[digit_display[digit_counter]]) >> 8;
    uint8_t b = (characters[digit_display[digit_counter]]) & 0xFF;
    SPI.transfer(b);
    SPI.transfer(a);
    digitalWrite(23, HIGH);
    */
    // digitalWrite(cs_pins[digit_counter], HIGH);
    next_digit();
}

} // namespace

