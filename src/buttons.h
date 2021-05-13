#pragma once

namespace kisse {

class buttons {
    public:
        void init();
        void pollButtons();
        int getEvent();
    private:
        int button_map[16] = {15,11,7,3,    14,10,6,2,    13,9,5,1,    12,8,4,0};
        bool button_matrix[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        void onToggle(uint16_t event);
        uint16_t button_state;
        uint16_t button_mask;
};

}