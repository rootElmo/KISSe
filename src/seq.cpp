#include <Arduino.h>
#include "seq.h"
#include <elapsedMillis.h>

sequence test_seq;

byte bpm = 120;                         
unsigned int bpm_millis = 15000 / bpm;  // One beat = 1 minute / beats per minute.
                                        // We can use this value later to determine PPQ (parts per quarter)
                                        // for clock output, to improve compatibility between devices.
bool play_on;
int seq_length = 16;
int step = -1;
bool first_step = true; // Do I really need first_step boolean atm?
elapsedMillis stepTime;

void seq::init() {
    play_on = false;
}

void seq::updateClock() {
    if (play_on == true && stepTime >= bpm_millis) {
        incrStep();
    }
}

void seq::onPlay() { // when play is pressed
    if (play_on == false && first_step) {
        play_on = true;
        stepTime = 0;
        incrStep();
    }
}

void seq::onStop() { // when stop is pressed
    if (play_on == true) {
        play_on = false;
        first_step = true;
        stepTime = 0;
        step = -1;
    } 
}

void seq::incrStep() { // increment step
    if (play_on == true) {
        step++;
        if (step >= seq_length) {
            step = 0;
            first_step = true;
        }
        stepTime = 0;
        first_step = false;
        Serial.print("Step value: ");
        Serial.println(test_seq.note_matrix[step]);
    }
}

bool seq::seqRunning() {
    return play_on; // ~~something funky going on here? ui.cpp reads this as true, not as the variable value~~
                    // Fixed above issue, didn't handle queue correctly.
                    // Need to learn more about queue and implement corrently for button events
}

void seq::incrTempo(int amnt) { // increment tempo
    bpm += amnt;
    if (bpm >= 420) {
        bpm = 420;
    } else if (bpm <= 10) {
        bpm = 10;
    }
    bpm_millis = 15000 / bpm;
    Serial.print("BPM = ");
    Serial.println(bpm);
}

bool seq::stepGateState(int gate) { // get gate state from sequence
    
}
