#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>
#include "scheduler.h"

#define CREATE_CHILD_PRIME_ADDER 6
#define NEW_DEVICE_CONNECTED 0
#define TOGGLE_LED 1

#define OFF 0
#define ON 1
#define DEVICE_ID 1

#define CONNECTED_DEVICE_I 13
#define CONNECTED_DEVICE_O 16
#define LED 2

void IRAM_ATTR device_disconnected_isr();

class Kernel {
    private:
        static boolean ledStatus;
    public:
        Scheduler scheduler;
        Kernel();
        uint8_t getConnectedDevices() const {return connectedDevices;}
        void handleMessage();
        static uint8_t id; //can only actually be 4 bits, not eight
        static uint8_t connectedDevices;


};


#endif