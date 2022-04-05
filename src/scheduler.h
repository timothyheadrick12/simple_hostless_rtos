#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<Arduino.h>
#include<program.h>

class Scheduler {
    private:
        Program ** scheduler;
    public:
        Scheduler();
        ~Scheduler();
        void push(byte data);
        void runScheduled();

};

#endif