#include <Arduino.h>
#include "ui.h"

// [K]eep [I]t [S]imple [SE]quencer
// Keep the main clean :)

ui myUi;
// Init the MCP23S17, CS pin = 10

void setup() { // pass sequencer to ui with init
    myUi.init();
}

void loop() {
    myUi.poll();
}

