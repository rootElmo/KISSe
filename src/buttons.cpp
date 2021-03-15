#include <Arduino.h>
#include "buttons.h"
#include "pins.h"
#include "SPI.h"
#include <gpio_MCP23S17.h>
#include "../lib/Arduino-Queue.h/Queue.h"

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
        // Mask off everything except the row (INPUT) pins (1111 0000 0000 0000).
        // The integer returned by the readGpioPort is a 16-bit representation
        // of the states of all the pins.

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
    // Read event from queue
    // When this gets moved to buttons.cpp,
    // It should only return the below value 
    // return uint16_t print = events.pop();
    uint16_t print = events.pop();
    // Serial.println(print, BIN);

    // The whole shebang under here could be moved to ui.cpp
    // later, since we only need to return the states of the buttons.
    // Classes which check for the presses should be the ones
    // doing whatever happens below.
    if ((print >> 8) == 1) { // Check the last bit for "ON" state (1 = ON, 0=OFF)
        if ((print & B01111111) < 16) { // button matrix has values below 16
            Serial.print("Button ");
            int buttonNum = (print & B00001111) + 1; // Shoudln't need the masking, keep just in case
            Serial.print(buttonNum);
            Serial.println(" pressed");
        } else { // Check the function buttons (values at or above 16)
            // In the future, there might be more
            // FUNC buttons, but these will do for
            // testing
            switch (print & B01111111)
            {
            case 16:
                    // Play
                    Serial.println("PLAY pressed");
                break;
            case 17:
                    // Stop
                    Serial.println("STOP pressed");
                break;
            case 18:
                    // Defined by user (me :D)
                    Serial.println("TBA pressed");
                break;
            }
        }
    }
    if (print == 0) {
        return;
    }
    // Below printing for debugging
    /*
    Serial.print("Event is: ");
    Serial.println(print, BIN);
    */
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