#include <Arduino.h>
#include "ui.h"
#include "pins.h"
#include "buttons.h"
#include "seq.h"

buttons my_buttons;
seq my_seq;

int print_incr;
int note_list [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Move everything "note" related to seq.cpp later
int note_selected = 0;
bool btn_state = false;

void ui::init() { // Get Sequencer from main.cpp with init!!!
    my_seq.init();
    my_buttons.init();
}

void ui::onEncIncr() {
    if (print_incr != 0) {
        my_seq.incrTempo(print_incr);
    }
}

void ui::onPlayBtn() {
    my_seq.onPlay();
}

void ui::onStopBtn() {
    my_seq.onStop();
}

void ui::poll() {
    
    my_buttons.pollButtons();
    uint16_t value = 0;
    if (my_buttons.getEvent(value) == 0) {
        int button = value & 0x00FF;
        bool button_state = (value & 0x800) >> 8;
        Serial.print("Button ");
        Serial.print(button);
        Serial.print(", value: ");
        Serial.println(value, BIN);
    }

}