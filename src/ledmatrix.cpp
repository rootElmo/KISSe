#include <Arduino.h>
#include "ledmatrix.h"
#include <SPI.h>
#include "seq.h"

namespace kisse {

// Get sequencer from main, like in ui.cpp etc.
bool led_matrix[17];
//  second = {3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 0}

// first = {0, 1, 2, 3,   0, 1, 2, 3,    0, 1, 2, 3,    0, 1, 2, 3,     0};
// 3rd = {0, 1, 2, 3,  3, 2, 1, 0,   0, 1, 2, 3,   3, 2, 1, 0, 0}

// 4th = {3, 3, 3, 3,   2, 2, 2, 2,   1, 1, 1, 1,   0, 0, 0, 0,   0}

//5th {3, 2, 1, 0,   3, 2, 1, 0,   3, 2, 1, 0,   3, 2, 1, 0,   0};
int step_map[17] = {3, 2, 1, 0,   3, 2, 1, 0,   3, 2, 1, 0,   3, 2, 1, 0,   0};
int selected_step_led = 0;
int row_counter = 0;
int led_col = 0;

uint8_t pins_byte;
uint8_t led_rows;

seq* sequencer1;

void leds::init(seq& sequencer) {
    pinMode(22, OUTPUT);
    SPI.begin();
    digitalWrite(22, HIGH);
    sequencer1 = &sequencer;
}

void leds::test_loop() {
    digitalWrite(22, LOW);
    SPI.setBitOrder(LSBFIRST);
    // Bxxxx0000; x = columns
    // B0000xxxx; x = rows
    SPI.transfer(B01111000);
    digitalWrite(22, HIGH);
    delay(100);
    digitalWrite(22, LOW);
    SPI.setBitOrder(LSBFIRST);
    SPI.transfer(B01110001);
    digitalWrite(22, HIGH);
    delay(100);
    
}

void leds::update(int row) {
    // COMPARE button_map[] IN buttons.h TO step_map[]!!!!!!!!!!
    // 
    pins_byte = (1 << (7 - row)); // B00001000 when row == 0
	led_rows = 0xF0;
	for (led_col = 0; led_col < 4; led_col++) {
		led_rows +=  (led_matrix[row * 4 + led_col] << step_map[row * 4 + led_col]); // 
	}
    pins_byte += ~led_rows;
    // Pin driver isn't inverted, all led rows are 1.

    // Serial.println(pins_byte, BIN);
    digitalWrite(22, LOW);
    SPI.setBitOrder(LSBFIRST);
    SPI.transfer(pins_byte);
    digitalWrite(22, HIGH);
}

void leds::multiplex() {
    update(row_counter);
    row_counter++;
    if (row_counter == 4) {
        row_counter = 0;
    }
}

// check if this function is ever called
// this in fact IS NEVER called, check super sixteen ui.cpp
// for reference
void leds::matrixFromSeq() {
    memcpy(led_matrix, sequencer1->getPtrnMatrix(), 16);
    selected_step_led = sequencer1->getSelectedStep();
    /*
    if (selected_step_led >= 16 || selected_step_led <= 0) {
        selected_step_led = 16;
    }
    */
}

}