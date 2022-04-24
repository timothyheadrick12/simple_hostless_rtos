//Kernel class. The kernel actually manages/implements the scheduler and handles recieved messages

#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>
#include "scheduler.h"

#define CREATE_CHILD_PRIME_ADDER 6//id used to interpret recieved messages
#define NEW_DEVICE_CONNECTED 0//unused in current implementation

#define DEVICE_ID 0 //unused in current implementation

#define CONNECTED_DEVICE_I 13 //unused in current implementation
#define CONNECTED_DEVICE_O 19 //unused in current implementation

class Kernel {
    public:
        Scheduler scheduler;
        Kernel();

        //descr: Handle any messages that may have come in the last OS cycle
        //post: messageRecieved set to false if a message was handled, any processes added to scheduler as needed
        void handleMessage();
        static uint8_t connectedDevices;//how many devices are currently connected to this device


};


#endif