#include <Arduino.h>
#include "seq.h"
#include <elapsedMillis.h>

namespace kisse {

sequence active_seq;

byte bpm = 120;                         
unsigned int bpm_millis = 15000 / bpm;  // One beat = 1 minute / beats per minute.
                                        // We can use this value later to determine PPQ (parts per quarter)
                                        // for clock output, to improve compatibility between devices.
bool play_on;
int seq_length = 16;
int step = -1;
bool first_step = true; // Do I really need first_step boolean atm?
int selected_step = 0;

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
        if (active_seq.gate_matrix[step] == true) { // If step is active (gate_matrix[] holds the states)
            Serial.print("Step ");
            Serial.print(step + 1);
            Serial.print(" active. ");
            Serial.print("Step value: ");
            Serial.println(active_seq.note_matrix[step]);
        } else if (active_seq.gate_matrix[step] == false) { // If step is inactive
            Serial.print("Step ");
            Serial.print(step + 1);
            Serial.println(" inactive. ");
        }
    }
}

bool seq::seqRunning() {
    return play_on;
}

void seq::setTempo(int amnt) { // increment tempo
    bpm += amnt;
    if (bpm >= 255) {
        bpm = 255;
    } else if (bpm <= 10) {
        bpm = 10;
    }
    bpm_millis = 15000 / bpm;
    Serial.print("BPM = ");
    Serial.println(bpm);
}

bool seq::stepGateState(int gate) { // get gate state from sequence
    // could use this when determining gate length?
    return 1;
}

void seq::selectStep(int step) { // something funky going on here
    // Steps 5, 9, 13 not activating while button pressed
    if (selected_step == step || active_seq.gate_matrix[step] == false) {
        active_seq.gate_matrix[step] = !active_seq.gate_matrix[step];
    }
    selected_step = step;
}

void seq::setPitch(int value) {
    active_seq.note_matrix[selected_step] += value;
}


// Check if this function is ever called????
bool *seq::getPtrnMatrix() {
    return active_seq.gate_matrix;
}

int seq::getSelectedStep() {
    return selected_step;
}

}