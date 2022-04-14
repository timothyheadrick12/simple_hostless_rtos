#ifndef PRIME_ADDER_H
#define PRIME_ADDER_H

#include <program.h>

class PrimeAdder: public Program {
    private:
        static uint8_t nextId;

        uint8_t parentDeviceId;
        //if parentProcessId is zero there is no parent instance
        uint8_t parentProcessId;
        uint8_t numChildProcesses;
        uint8_t numResponses;
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
        PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const uint8_t & m_parentDeviceId,const uint8_t & m_parentProcessId);
        PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const Kernel & kernel);
        void execute(const double & numCycles);
        void execute();
        void handleResponse(const uint32_t & response);
};

#endif