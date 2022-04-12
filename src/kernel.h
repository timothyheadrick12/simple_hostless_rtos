#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>

class Kernel {
    private:
        byte connectedDevices;
    public:
        Kernel();
        static byte id;
};


#endif