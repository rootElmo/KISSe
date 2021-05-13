#include <Arduino.h>
#include "ui.h"
#include "seq.h"
#include "display.h"
#include "dac.h"

using namespace kisse;

// [K]eep [I]t [S]imple [SE]quencer
// Keep the main clean :)

ui myUi;
seq my_seq;
display my_display;
dac main_dac;
// Init the MCP23S17, CS pin = 10

void setup() {  // pass sequencer to ui with init
                // This needs some pointer magic to be working
                // We need to pass an instance of sequencer to the
                // ui instead of declaring it here, and in ui.cpp.
                // Then we would have two instances of sequencer
                // and we wouldn't be able to talk to the same instance
                // of sequencer

    /*
    my_seq.init();
    myUi.init(my_seq);
    */
   
    main_dac.init();

    // my_display.init();
}

void loop() {
    
    // my_display.driver();
    // my_display.update();
    

    // my_display.test_flash();
    main_dac.dac_out();
    /*
    myUi.poll();
    myUi.multiplex();
    if (myUi.isPlaying() == true) {
        my_seq.updateClock();
    }
    */

   // my_leds.one_by_one();
}

