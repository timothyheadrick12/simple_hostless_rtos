#include "led_toggler.h"

boolean LedToggler::ledStatus = OFF; 

void LedToggler::execute(const double & numCycles) {
    if(ledStatus) {
        digitalWrite(LED, LOW);
        ledStatus = OFF;
        Serial.println("Turned off LED");
    } else {
        digitalWrite(LED, HIGH);
        ledStatus = ON;
        Serial.println("Turned on LED");
    }
    complete = true;
}