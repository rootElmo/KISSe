#pragma once

class ui {
    public:
        void init();
        void poll();
        void onPlayBtn();
        void onStopBtn();
    private:
        void onEncIncr();
};