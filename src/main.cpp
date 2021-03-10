#include <Arduino.h>
#include "ui.h"
#include "SPI.h"
#include <gpio_MCP23S17.h>
#include "elapsedMillis.h"

// [K]eep [I]t [S]imple [SE]quencer
// Keep the main clean :)

// Testin button matrix scanning in main for now. Move to
// buttons.cpp when done. Also clean up variable names etc.

// Can't actually tell if the debouncing is helping that much? Or is it working?
// Could be an issue in the way the switches' mechanical construction
#define DEBOUNCE_TIME 20

ui myUi;
// Init the MCP23S17, CS pin = 10
gpio_MCP23S17 mcp(10,0x20);

int row = 0;
uint16_t button_state;
uint16_t button_mask;
// int button_map[] = {4, 8, 12, 16,    3, 7, 11, 15,    2, 6, 10, 14,    1, 5, 9, 13,};
int button_map[] = {16, 12, 8, 4,    15, 11, 7, 3,    14, 10, 6, 2,    13, 9, 5, 1,};
bool button_matrix[] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
elapsedMillis debounce_timer;

void onPress(bool state, int button) {
    if (debounce_timer < DEBOUNCE_TIME) {
        return;
    }
    if (state == 1) {
        if (button_matrix[button] == state) {
            return;
        }
        Serial.print("Button ");
        Serial.print(button);
        Serial.println(" pressed.");
        debounce_timer = 0;
    } else if (state == 0) {
        if (button_matrix[button] == state) {
            return;
        }
        Serial.print("Button ");
        Serial.print(button);
        Serial.println(" released.");
        debounce_timer = 0;
    }
}

void pollButtons() {
    
    row++;
    if (row > 3) {
        row = 0;
    }

    mcp.gpioDigitalWrite(row+8, HIGH); // Set the column to be scanned HIGH (change int row to col??)

    uint16_t button_val = mcp.readGpioPort(); // Read all pins of MCP23S17 (return an int)
    // Serial.println(button_val, BIN);
    // Change o to something else? Looks too much like 0 or O
    for (int o = 0; o < 4; o++) {
        int button_num = button_map[row * 4 + o];
        
        uint16_t button_state = button_val & 0xF000; // DO NOT EDIT button_val DIRECTLY!!!!
        // Mask off everything except the row (INPUT) pins (1111 0000 0000 0000).
        // The integer returned by the readGpioPort is a 16-bit representation
        // of the states of all the pins.

        button_state = button_state >> 12; // Shift the input pins to "zero" position (1111)
        button_state = button_state & 0x01 << o; // Select one bit to read depending on "o" (1111 & 0x01 = 0001 when o = 0)
        bool value = (button_state >> o);   // Shift the selected value to "zero" position  
        
        if (value != button_matrix[button_num]) { // If the value of the read pin is different than the one in button_matrix....
            onPress(value, button_num);
            button_matrix[button_num] = value; // Set new value to matrix
            /*
            Serial.print("Button ");
            Serial.print(button_num);
            Serial.println(" changed");
            */
        }
    }
    mcp.gpioDigitalWrite(row+8, LOW);
}

void setup() { // pass sequencer to ui with init
    // myUi.init();
    mcp.begin();
    mcp.gpioPort(0x0000);
    for (int ii = 0; ii < 4; ii++) {
        mcp.gpioPinMode(ii + 8, OUTPUT);
        mcp.gpioPinMode(ii + 12, INPUT);
    }
    mcp.gpioPort(LOW);

    pollButtons();
    pollButtons();
    pollButtons();
    pollButtons();
}

void loop() {
    // myUi.poll();
    pollButtons();
}

