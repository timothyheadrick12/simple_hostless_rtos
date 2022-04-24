#ifndef PRIME_ADDER_H
#define PRIME_ADDER_H

#include "program.h"
#include "kernel.h"

class PrimeAdder: public Program {
    private:
        //anything with id 8 or greater is a primeAdder
        //ids 0-7 are reserved for potential kernel processes
        static uint8_t nextId;//What the next id of a prime adder should be

        unsigned long processStartTime;//Used to determine how long process took in total to execute upon completion
        
        uint8_t parentDeviceId;//not used in current implementation of OS such multiple devices are not supported
        
        uint8_t parentProcessId; //if parentProcessId is zero there is no parent instance
        uint8_t numChildProcesses; //how many, if any, child processes were created on other devices by this process
        uint8_t numResponses; //how many, if any, child processes created on other devices have completed and responded
        
        //the total sum of prime numbers between initial curVal and 
        //maxVal inclusive
        uint32_t sum; 
        //the maximum value to be tested if prime
        uint16_t maxVal; 
        //the current value less than equal to maxVal being tested if prime
        uint16_t curVal;
        //the current value been used to test if curVal is prime
        uint16_t curTestVal;
    public:
        //pre: 2 <= curVal <= maxVal < 10,000
        PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const uint8_t & m_parentDeviceId,const uint8_t & m_parentProcessId);//used to create child processes
        PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal);//used to create parent processes
        
        //descr: Performs the execute function numCycles times
        void execute(const double & numCycles);
        
        //descr: Tests a single curTestVal and controls process completion
        //pre: none
        //post: curTestVal or curVal incremented, process set to complete, or process set to WAITING
        void execute();

        //descr: Handle a child process's sum result being returned
        //pre: message targeting proces recieved
        //post: response added to sum
        void handleResponse(const uint32_t & response);
};

#endif