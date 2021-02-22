#include <Arduino.h>
#include "ui.h"
#include "encoder.h"
#include "pins.h"
#include "buttons.h"

encoder test_enc;
buttons my_butt; // big funny

int print_incr;
int note_list [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Move everything "note" related to seq.cpp later
int note_selected = 0;
bool btn_state = false;

void ui::init() { // Pins defined at pins.h 
    test_enc.init();
    my_butt.init();
    pinMode(ENC_BTN, INPUT); // MOVE TO BUTTONS-CPP
    pinMode(PRINT_BTN, INPUT); // MOVE TO BUTTONS-CPP
}

void ui::onEncIncr() { 
    if (digitalRead(ENC_BTN) == LOW) { // If encoder button is pressed, set note value
        note_list[note_selected] += print_incr; // move this whole shebang to buttons.cpp,
        Serial.print("Note value: ");           // since the encoder button IS A BUTTON
        Serial.println(note_list[note_selected]);
    } else if (digitalRead(ENC_BTN) == HIGH) { // if encoder button is not pressed, set selected note
        note_selected += print_incr;
        if (note_selected >= 16) {
            note_selected = 0;
        }
        if (note_selected <= -1) {
            note_selected = 15;
        }
        Serial.print("Note selected: ");
        Serial.println(note_selected);
    }
}

void ui::printNotes() { // print note_list notes
    if (digitalRead(PRINT_BTN) == HIGH) {
        if (btn_state) {
            btn_state = false;
        }
        return;
    }
    if (digitalRead(PRINT_BTN) == LOW) {
        if (btn_state) {
            return;
        }
        btn_state = true;
        Serial.println("Pattern notes:"); // Same logic could be used in seq.cpp later
        for (int i = 0; i <= 15; i++) { // "i" could be defined by something like "MAX_STEPS" in the future
            Serial.print("[");          // to only scan notes defined by pattern length
            Serial.print(note_list[i]);
            if (i == 15) {
                Serial.println("]");
                return;
            }
            Serial.print("], ");
        }
    }
}

void ui::poll() {
    print_incr = test_enc.poll();
    if (print_incr != 0) {
        onEncIncr(); // If encoder is incremented, handle increment
    }
    printNotes(); // poll buttons, call this onBtnPress (again, stuff that should be on buttons.cpp)
    my_butt.poll();
}