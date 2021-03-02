#include <Arduino.h>
#include "ui.h"
#include "encoder.h"
#include "pins.h"
#include "buttons.h"
#include "seq.h"

// TODO: push button presses to queue

encoder test_enc;
buttons my_butt; // big funny
seq my_seq;

int print_incr;
int note_list [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Move everything "note" related to seq.cpp later
int note_selected = 0;
bool btn_state = false;

void ui::init() { // Get Sequencer from main.cpp with init!!!
    test_enc.init();
    my_butt.init();
    my_seq.init();
    pinMode(ENC_BTN, INPUT); // MOVE TO BUTTONS-CPP
    pinMode(PRINT_BTN, INPUT); // MOVE TO BUTTONS-CPP
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

    // Serial.println(my_seq.seqRunning());
    if (my_seq.seqRunning()) { // If "play_on" = true in seq.cpp, update clock!
        my_seq.updateClock();
    }
    print_incr = test_enc.poll();
    if (print_incr != 0) {
        onEncIncr(); // If encoder is incremented, handle increment
    }
    my_butt.poll();
    if (my_butt.getQueEvent() == true) {
        // Serial.println("Play in queue");
        // Play button is pressed
        my_seq.onPlay();
    } else if (my_butt.getQueEvent() == false) {
        // Serial.println("Stop in queue");
        // Stop button is pressed
        my_seq.onStop();
    }
}