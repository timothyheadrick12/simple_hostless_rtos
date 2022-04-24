#include "led_toggler.h"

boolean LedToggler::ledStatus = OFF; //start with LED off

void LedToggler::execute(const double & numCycles) {
    //if LED  is on, turn off
    if(ledStatus) {
        digitalWrite(LED, LOW);
        ledStatus = OFF;
        Serial.println("Turned off LED");
    } else {//if LED is off, turn on
        digitalWrite(LED, HIGH);
        ledStatus = ON;
        Serial.println("Turned on LED");
    }
    complete = true;//mark process as complete
}