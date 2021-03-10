#include <Arduino.h>
#include "buttons.h"
#include "pins.h"
#include <cppQueue.h>
// #include "SPI.h"
// #include <gpio_MCP23S17.h>

// int btn_map[] = {PLAY_BTN, STOP_BTN, BTN1, BTN2};
// cppQueue queue(3, 10, FIFO);
// Queue not used now, need to study it more, implement correctly. Wouldn't pop events correctly last time.
// NEED TO ADD DEBOUNCE TO BUTTONS
// MOVE ENCODER BUTTON AND PRINT BUTTON
// FROM ui.cpp TO HERE!
int btn_event;

// gpio_MCP23S17 mcp(10,0x20);

void buttons::init() {
    /*
    pinMode(PLAY_BTN, INPUT);
    pinMode(STOP_BTN, INPUT);
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    
    mcp.begin();
    for (int i = 0; i < 4; i++) {
        // Set mcp23s17 output and input pins (1 = in; 0 = out)
        // mcp.gpioPinMode(i, 1);
        mcp.gpioPinMode(i+4, 1);
        // mcp.gpioPinMode(i+8, 1);
        // mcp.gpioPinMode(i+12, 1);
    }
    */
    poll();
    // get poll() here 1 time to save BUTTON STATES TO btn_state_matrix[]
}

void buttons::poll() { // Scan buttons for state change
    /*
    for (int ii = 0; ii < 4; ii++) {
        mcp.gpioDigitalWrite(ii+4, HIGH);
    }
    
    int test = mcp.readGpioPort();
    Serial.print("Button values: ");
    Serial.println(test);

    */

}

void buttons::getButtonEvent(int button, bool state) { //WORKS LIKE A VOID FOR NOW, USE RETURN VALUE TO CONTROL SEQ
    // "bool state" doesn't have any proper role as of now,
    // but will be used in the future
    // Switch syntax
    // https://www.arduino.cc/reference/en/language/structure/control-structure/switchcase/
    switch (button){
        case 0:
            if (state == false) {
                Serial.println("Play!");
                btn_event = true;
                // onPlay();
                break;
            } else {
                break;
            }
        case 1:
            if (state == false) { // if state is false
                Serial.println("Stop!");
                btn_event = false;
                // onStop();
                break;
            } else {
                break;
            }
        default:
            break;
    }
}

// Learn more about this. Library has some bitwizardry with pointers
/*
void buttons::onPlay() {
    queue.push(0);
}

void buttons::onStop() {
    queue.push(1);
}
*/

bool buttons::getQueEvent() {
    return btn_event;
}
