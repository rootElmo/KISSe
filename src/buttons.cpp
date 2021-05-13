#include <Arduino.h>
#include "buttons.h"
#include "pins.h"
#include "SPI.h"
#include <gpio_MCP23S17.h>
#include "../lib/Arduino-Queue.h/Queue.h"

namespace kisse {

#define DEBOUNCE_TIME 20

gpio_MCP23S17 mcp(10,0x20);

Queue<uint16_t> events = Queue<uint16_t>(16);

int col = 0;

int func_button_map[] = {0, 1, 2};
bool func_matrix[] = {0, 0, 0};

elapsedMillis debounce_timer;
volatile uint16_t bouncing_button = 0x00;

void buttons::init() {
    mcp.begin();
    for (int ii = 0; ii < 4; ii++) {
        mcp.gpioPinMode(ii + 8, OUTPUT);
        mcp.gpioPinMode(ii + 12, INPUT);
    }
    
    mcp.gpioPinMode(5, INPUT);
    mcp.gpioPinMode(6, INPUT);
    mcp.gpioPinMode(7, INPUT);
    
    mcp.gpioPort(LOW);

    pollButtons();
    pollButtons();
    pollButtons();
    pollButtons();
}

void buttons::pollButtons() {
    col++;
    if (col > 3) {
        col = 0;
    }
    mcp.gpioDigitalWrite(col+8, HIGH);

    uint16_t button_val = mcp.readGpioPort(); // Read all pins of MCP23S17 (return an int)
    // Serial.println(button_val, BIN);
    for (int ii = 0; ii < 4; ii++) {
        int button_num = button_map[col * 4 + ii];
        
        uint16_t button_state = button_val & 0xF000;

        button_state = button_state >> 12; // Shift the input pins to "zero" position (1111)
        button_state = button_state & 0x01 << ii; // Select one bit to read depending on "o" (1111 & 0x01 = 0001 when o = 0)
        bool value = (button_state >> ii);   // Shift the selected value to "zero" position  

        if (value != button_matrix[button_num]) { // If the value of the read pin is different than the one in button_matrix....
            button_matrix[button_num] = value; // Set new value to matrix
            uint16_t event = button_num | value << 8;
            onToggle(event);
        }
    }
    
    // Function button scanning, quite similiar to the matrix scanning
    for (int ii = 0; ii < 3; ii++) {
        uint16_t button_state = button_val & 0x00F0;

        button_state = button_state >> 4;
        button_state = button_state & 0x01 << ii;
        bool value = (button_state >> ii);

        if (value != func_matrix[ii]) {
            // onPress(value, func_button_map[ii]); // Do i need onPress, or could the corresponding function be called immediately? (play, stop, print)
            // Actually do what I describe above, seems the most practical way
            func_matrix[ii] = value;
            // Set function buttons to values > 16
            // so that they may be easily
            // differentiated from button matrix
            // buttons :)
            uint16_t event = (func_button_map[ii] + 16) | (value << 8);
            onToggle(event);
        }
    }
    mcp.gpioDigitalWrite(col+8, LOW);
}

int buttons::getEvent() {
    return events.pop();
}

void buttons::onToggle(uint16_t event){
    // Debounce, and queue pushing function
    // practically copied straight from
    // Super Sixteen. Couldn't have done it better myself
    if (debounce_timer < DEBOUNCE_TIME && (event & 0x00FF) == bouncing_button) {
        return;
    }
    events.push(event);
    bouncing_button = event & 0x00FF;
    debounce_timer = 0;
}

}