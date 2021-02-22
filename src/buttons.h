#pragma once

class buttons {
    public:
        void init();
        void poll();
        void getButtonEvent(int button, bool state); // change return type to int later
    private:
        bool btn_matrix[4] = {1, 1, 1, 1};
        bool btn_status_matrix[4]; // SAVE LAST STATE HERE!!!!
};