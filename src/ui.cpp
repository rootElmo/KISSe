#include <Arduino.h>
#include "ui.h"
#include "pins.h"
#include "buttons.h"
#include "seq.h"
#include "encoder.h"
#include "ledmatrix.h"

namespace kisse {

buttons my_buttons;
encoder my_enc;
leds my_leds;

seq *my_seq;

// change print_incr name to something more appropriate
int enc_state;
int note_list [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Move everything "note" related to seq.cpp later
bool btn_state = false;
bool is_playing = false;

int param_select = 0; // 0 = pitch; 1 = tempo; (add more as needed)
int prev_param = 0;

// https://forum.arduino.cc/index.php?topic=511070.0
void ui::init(seq& sequencer) { // Get Sequencer from main.cpp with init!!!
    my_seq = &sequencer;
    my_buttons.init();
    my_enc.init();
    my_leds.init(sequencer);
}

// https://forum.arduino.cc/index.php?topic=511070.0
void ui::onEncIncr() {
        // "->" in this case works the same as "."
        // but we are "getting" my_seq from main.cpp
        // as a pointer.
    switch (param_select) {
    case 0: // PITCH/NOTE
        my_seq->setPitch(enc_state);
        break;
    case 1: // TEMPO
        my_seq->setTempo(enc_state);
        break;
    }
}

void ui::onPlayBtn() {
    my_seq->onPlay();
    is_playing = true;
}

void ui::onStopBtn() {
    my_seq->onStop();
    is_playing = false;
}

bool ui::isPlaying() {
    return is_playing;
}

void ui::poll() {
    
    my_buttons.pollButtons();
    uint16_t value = my_buttons.getEvent();
    // getEvent returns buttons in bits where
    // x00000000; x = button state
    // 00000xxxx; x = button matrix buttons
    // 0xxxx0000; x = function buttons
    // (leading 0 bits omitted)
    int button = value & 0x00FF;
    bool button_state = value >> 8;
    onToggle(button_state, button);

    enc_state = my_enc.poll();
    if (enc_state != 0) {
        // do what you do with encoder increments
        onEncIncr();
    }
}

void ui::onToggle(bool state, int button) {
    if (state == 1) {
        if (button < 16) { // Button matrix
            Serial.print("Button ");
            Serial.print(button + 1);
            Serial.println(" pressed!");
            selectStep(button);
        }
        switch (button) {
        case 16: // PLAY
            onPlayBtn();
            Serial.println("PLAY");
            break;
        case 17: // STOP
            onStopBtn();
            Serial.println("STOP");
            break;
        case 18: // TEMPO
            // Currently TEMPO is the only parameter state
            // (besides note edit)
            if (param_select == 1) { // when function state is active AND pressed again
                param_select = prev_param; // return to previous parameter state
                break;
            }
            prev_param = param_select; // save last parameter mode
            param_select = 1; // change parameter mode

            Serial.println("FUNC");
            break;
        }
    }
}

void ui::selectStep(int step) {
    my_seq->selectStep(step);
    my_leds.matrixFromSeq();
}

void ui::multiplex() {
    my_leds.multiplex();
}

}