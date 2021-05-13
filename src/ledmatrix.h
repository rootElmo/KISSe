#pragma once

#include <Arduino.h>
#include "seq.h"

namespace kisse {

class leds {
    public:
        void init(seq &sequencer);
        void test_loop();
        void one_by_one();
        void matrixFromSeq();
        void multiplex();
        void update(int row);
};

}