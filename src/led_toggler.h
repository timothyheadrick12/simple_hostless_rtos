#ifndef LED_TOGGLER_H
#define LED_TOGGLER_H

#include "program.h"

#define OFF 0
#define ON 1
#define TOGGLE_LED 1
#define LED 2

class LedToggler: public Program {
    private:
        static boolean ledStatus;
    public:
        LedToggler(): Program(1) {}
        void execute(const double & numCycles);
        void handleResponse(const uint32_t & response) {};
};


#endif