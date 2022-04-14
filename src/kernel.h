#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>

#define CREATE_CHILD_PRIME_ADDER 6

class Kernel {
    private:
        uint8_t connectedDevices;
    public:
        Kernel() {}
        uint8_t getConnectedDevices() const {return connectedDevices;}
        void handleMessage();
        static uint8_t id; //can only actually be 4 bits, not eight

};


#endif