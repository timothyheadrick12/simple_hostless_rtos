#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>

class Program {
    protected:
        byte pc;
        boolean complete;
    public:
        Program(): pc(0), complete(false) {}
        boolean isComplete() {return complete;}
        virtual void execute(byte numCycles) = 0;
};

#endif