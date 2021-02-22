#include <Arduino.h>
#include "seq.h"
// Check elapsedmillis.h library

sequence test_seq;

byte bpm = 120;
unsigned int bpm_millis = 15000 / bpm; // 15000 to be replaced by PPQ setting (parts per quarter)
bool play_on;

void seq::init() {

}

void seq::onPlay() { // when play is pressed

}

void seq::onStop() { // when stop is pressed

}

void seq::incrStep() { // increment step

}

int seq::incrTempo(int amnt) { // increment tempo

}

bool seq::stepGateState(int gate) { // get gate state from sequence

}
