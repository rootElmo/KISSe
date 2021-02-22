#pragma once

class encoder {

    public:
        void init();
        int poll();
        int enc_amnt = 0;
        int incr_amnt = 0;    
};