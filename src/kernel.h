#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>
#include "scheduler.h"

#define CREATE_CHILD_PRIME_ADDER 6
#define NEW_DEVICE_CONNECTED 0
#define TOGGLE_LED 1

#define OFF 0
#define ON 1
#define DEVICE_ID 0

#define CONNECTED_DEVICE_I 13
#define CONNECTED_DEVICE_O 19
#define LED 2

class Kernel {
    private:
        static boolean ledStatus;
    public:
        Scheduler scheduler;
        Kernel();
        void handleMessage();
        static uint8_t connectedDevices;
        void toggleLed();


};


#endif