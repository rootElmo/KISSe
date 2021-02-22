#include <Arduino.h>
#include "buttons.h"
#include "pins.h"

int btn_map[] = {PLAY_BTN, STOP_BTN, BTN1, BTN2};

// NEED TO ADD DEBOUNCE TO BUTTONS
// MOVE ENCODER BUTTON AND PRINT BUTTON
// FROM ui.cpp TO HERE!

void buttons::init() {
    pinMode(PLAY_BTN, INPUT);
    pinMode(STOP_BTN, INPUT);
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);

    poll();
    // get poll() here 1 time to save BUTTON STATES TO btn_state_matrix[]
}

void buttons::poll() { // Scan buttons for state change
    for (int i = 0; i <= 3; i++) {
        btn_status_matrix[i] = digitalRead(btn_map[i]);
        // Needs to know last state
        // to prevent EXTRA LONG PRINTS
        // We don't need to constantly return the button state,
        // just as it's changing
        if (btn_status_matrix[i] != btn_matrix[i]) {
            getButtonEvent(i, btn_status_matrix[i]);
            btn_matrix[i] = btn_status_matrix[i];
            // call a function after setting buttons to make an event
        }
    }
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
                break;
            } else {
                break;
            }
        case 1:
            if (state == false) { // if state is false
                Serial.println("Stop!");
                break;
            } else {
                break;
            }
        default:
            break;
    }
}