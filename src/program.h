//Abstract base class for programs

#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>
#include "message.h"

//one funny problem is there is a finite amount of 
//processes that can be generated in the lifetime
//of the rtos ie 256 - 8

//aliases for controlling process state
#define READY 0
#define WAITING 1

class Program {
    protected:
        uint8_t id;
        uint8_t status;//should be set to WAITING when waiting for information from another device
        boolean complete;//must be set true when the process is complete
    public:
        Program(const uint8_t & m_id): id(m_id), status(READY), complete(false) {}
        boolean isComplete() const {return complete;}
        boolean isReady() const {return status == READY;}
        uint8_t getId() const {return id;}

        //descr: What a process should do when it is told to execute for some number of cycles.
        //       Should generally be a brief task as to not consume to much cpu time.
        //       This function should also eventually set the process to complete.
        virtual void execute(const double & numCycles) = 0;

        //descr: What a process should do when it recieves a message from another device targeting it.
        virtual void handleResponse(const uint32_t & response) = 0;
};

#endif