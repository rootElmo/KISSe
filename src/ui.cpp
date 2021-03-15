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
    int btn_value = 0;
    my_buttons.pollButtons();
    my_buttons.getEvent();

}