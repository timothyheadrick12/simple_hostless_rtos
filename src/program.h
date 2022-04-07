#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>

//one funny problem is there is a finite amount of 
//programs that can be generated in the lifetime
//of the rtos
class Program {
    protected:
        uint16_t id;
        byte pc;
        boolean complete;
    public:
        Program(uint16_t m_id): id(m_id), pc(0), complete(false) {}
        boolean isComplete() {return complete;}
        virtual void execute(byte numCycles) = 0;
        virtual void handleMessage(byte* message) = 0;
};

#endif