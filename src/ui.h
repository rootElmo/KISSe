#pragma once

// Has to be included, otherwise compiles errors!
// Not exactly sure why, since in the Super Sixteen
// the init(seq& sequencer) doesn't seem to require
// including seq.h in the header

#include "seq.h"

namespace kisse{

class ui {
    public:
        void init(seq& sequencer);
        void poll();
        void onPlayBtn();
        void onStopBtn();
        bool isPlaying();
        void multiplex();
    private:
        void onEncIncr();
        void onToggle(bool state, int button);
        void selectStep(int step);
};

}