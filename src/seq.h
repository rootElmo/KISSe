#pragma once

#include <Arduino.h>

namespace kisse {

struct sequence {
    int8_t note_matrix[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}; // values defined for testing purposes
    bool gate_matrix[16] = {1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    uint8_t tempo = 120;
};

class seq {
    public:
        void init();
        void incrStep();
        bool stepGateState(int gate);
        void updateClock();

        void onPlay();
        void onStop();
        bool seqRunning();

        void setPitch(int value);
        void setTempo(int amnt);

        void selectStep(int step);
        bool *getPtrnMatrix();
        int getSelectedStep();
};

}