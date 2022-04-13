#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>
#include "kernel.h"

//one funny problem is there is a finite amount of 
//programs that can be generated in the lifetime
//of the rtos
class Program {
    protected:
        uint16_t id;
        byte pc;
        boolean complete;
    public:
        Program(const uint8_t & m_id): id(m_id), pc(0), complete(false) {}
        boolean isComplete() const {return complete;}
        virtual void execute(const double & numCycles) = 0;
        virtual void handleMessage(byte* message) = 0;
        uint8_t getId() const {return id;}
};

#endif