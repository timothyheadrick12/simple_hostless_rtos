#include "prime_adder.h"

void PrimeAdder::execute(byte numCycles) {
    byte i = 0;
    while(i < numCycles && !complete) {
        execute();
    }
}

void PrimeAdder::execute() {
    if(curVal % curTestVal == 0) {
        curVal++;
        curTestVal = 2;
    } else {
        curTestVal++;
        if(curTestVal > curVal / 2) {
            sum += curVal;
            curVal++;
            curTestVal = 2;
        }
    }
    if(curVal > maxVal) {
        complete = true;
    }
}
