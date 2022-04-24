//process that handles toggling the on board LED.

#ifndef LED_TOGGLER_H
#define LED_TOGGLER_H

#include "program.h"

//-----aliases for whether an LED is on or off
#define OFF 0
#define ON 1


#define TOGGLE_LED 1 //process id
#define LED 2 //LED pin

class LedToggler: public Program {
    private:
        static boolean ledStatus;//is the on board led on or off
    public:
        LedToggler(): Program(TOGGLE_LED) {}

        //descr: toggles the on board LED, this is a quick task so only expected to take one "CPU" cycle
        //       (in reality this takes multiple CPU cycles, an OS cycle may be a better way to refer to it)
        void execute(const double & numCycles);
        
        //not needed for this process. This probably shouldn't be a true virtal function
        void handleResponse(const uint32_t & response) {};
};


#endif