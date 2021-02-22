#pragma once

#include <Arduino.h>

struct sequence {
    int8_t note_matrix[16];
    bool gate_matrix[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint8_t tempo = 120;
};

class seq {
    public:
        void init();
        void incrStep();
        bool stepGateState(int gate);
        int incrTempo(int amnt);

        void onPlay();
        void onStop();
};