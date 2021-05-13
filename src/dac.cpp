#include <Arduino.h>
#include "dac.h"
#include "SPI.h"

namespace kisse {

int dac_vol = 100;
int test_counter = 0;

void dac::init() {
    SPI.begin();
    pinMode(19, OUTPUT);
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    digitalWrite(19, LOW);
}

void dac::dac_out() {
    // https://ww1.microchip.com/downloads/en/DeviceDoc/20002249B.pdf
    // 5.2 "Write command" vvvvvvvvvvvvvvvvvv
    // B x000 0000 0000 0000 0000; dac select - 0 = DACA, 1= DACB
    // B 0x00 0000 0000 0000 0000; Dont care
    // B 00x0 0000 0000 0000 0000; gain select - 1 = 1x, 0 = 2x
    // B 000x 0000 0000 0000 0000; power down select - 1 = output power down, 0 = Output buffer disabled, Output is high-impedance
    // B 0000 xxxx xxxx xxxx xxxx;
    test_counter++;
    if (test_counter > 3) {
        test_counter = 0;
    }
    
    if (test_counter % 2 == 0) {
        digitalWrite(19, HIGH);
    } else {
        digitalWrite(19, LOW);
    }
    
    SPI.setBitOrder(MSBFIRST);
    digitalWrite(21, LOW);
    SPI.transfer(B00111111);
    SPI.transfer(B11111111);
    digitalWrite(21, HIGH);

    delay(500);
    
    SPI.setBitOrder(MSBFIRST);
    digitalWrite(21, LOW);
    SPI.transfer(B00110000);
    SPI.transfer(B00010000);
    digitalWrite(21, HIGH);
    delay(500);
    // https://www.attackmagazine.com/features/long-read/the-genesis-of-synthesis/
    // Basis for digitally controlled analog slide
}

void dac::update_output() {
    dac_vol += 10;
    if (dac_vol > 4095) {
        dac_vol = 100;
    }
    dac_out();
}

}