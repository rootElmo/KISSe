#pragma once

#include <Arduino.h>

namespace kisse {

class dac {
    public:
        void init();
        void dac_out();
        void update_output();
};

}

