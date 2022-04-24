//Scheduler class. Acts more like a storage container for running processes rather than actually handling any scheduling.
// The kernel itself is meant to manage the scheduler as needed to implement whatever scheduler algorithm is desired.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<Arduino.h>
#include "program.h"

#define schedulerLevels 7
#define levelLen 20
class Scheduler {
    private:
        //2d array of pointers to programs/processes
        Program *scheduler[schedulerLevels][levelLen];
    public:
        Scheduler();
        // ~Scheduler(); This should be implemented
        
        //descr: Push a program to the first level of the scheduler.
        //post: program added to the first available slot of the first level of the scheduler.
        void push(Program * program);

        //descr: Push a program to the given level of the scheduler.
        //post: program added to the first available slot of the given level of the scheduler.
        void push(Program* program, const uint8_t & level);

        //descr: Removes a given program from the scheduler. This generally shouldn't be needed
        //       since processes are removed by getNext.
        void remove(Program * program);

        //descr: Returns a pointer to the first READY process on a given level of the scheduler
        //       or a nullptr if process not found. Returned process is removed from the scheduler.
        Program* getNext(const uint8_t & level);

        //descr: Finds and returns a process given an id, this does not remove the process from the scheduler
        Program* find(const uint8_t & id) const;

};

#endif