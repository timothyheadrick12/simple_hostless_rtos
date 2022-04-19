#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>
#include "message.h"

//one funny problem is there is a finite amount of 
//programs that can be generated in the lifetime
//of the rtos

#define READY 0
#define WAITING 1

class Program {
    protected:
        uint8_t id;
        uint8_t status;
        boolean complete;
    public:
        Program(const uint8_t & m_id): id(m_id), status(READY), complete(false) {}
        boolean isComplete() const {return complete;}
        boolean isReady() const {return status == READY;}
        virtual void execute(const double & numCycles) = 0;
        uint8_t getId() const {return id;}
        virtual void handleResponse(const uint32_t & response) = 0;
};

#endif