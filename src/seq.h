#pragma once

#include <Arduino.h>

struct sequence {
    int8_t note_matrix[16] = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16}; // values defined for testing purposes
    bool gate_matrix[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint8_t tempo = 120;
};

class seq {
    public:
        void init();
        void incrStep();
        bool stepGateState(int gate);
        void incrTempo(int amnt);
        void updateClock();

        void onPlay();
        void onStop();
        bool seqRunning();
};