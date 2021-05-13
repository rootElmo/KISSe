#pragma once

#include <Arduino.h>

namespace kisse {

class display {
    public:
        void init();
        void test_flash();
        void next_digit();
        void set_num(int num);
        void update();
        void driver();
};

}