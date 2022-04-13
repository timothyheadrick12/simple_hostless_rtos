#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<Arduino.h>
#include<program.h>

#define schedulerLevels 7
#define levelLen 20
class Scheduler {
    private:
        Program *scheduler[schedulerLevels][levelLen];
    public:
        Scheduler();
        ~Scheduler();
        void push(Program* program);
        void push(Program* program, const uint8_t & level);
        Program* getNext(const uint8_t & level);
        Program* find(const uint16_t & id) const;

};

#endif