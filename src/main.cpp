#include <Arduino.h>
#include "ui.h"

// [K]eep [I]t [S]imple [SE]quencer
// Keep the main clean :)

ui myUi;

void setup() {
    myUi.init();
}

void loop() {
    myUi.poll();
}

