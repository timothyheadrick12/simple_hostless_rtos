#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>
#include "message.h"

#define CREATE_CHILD_PRIME_ADDER 6

class Kernel {
    private:
        uint8_t connectedDevices;
    public:
        Kernel();
        uint8_t getConnectedDevices() const {return connectedDevices;}
        static uint8_t id;

};


#endif