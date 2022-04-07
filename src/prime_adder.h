#ifndef PRIME_ADDER_H
#define PRIME_ADDER_H

#include <program.h>

class PrimeAdder: public Program {
    private:
        static uint16_t numInstances;

        //if parentId is zero there is no parent instance
        uint16_t parentId;
        //the total sum of prime numbers between initial curVal and 
        //maxVal inclusive
        unsigned long int sum; 
        //the maximum value to be tested if prime
        unsigned int maxVal; 
        //the current value less than equal to maxVal being tested if prime
        unsigned int curVal;
        //the current value been used to test if curVal is prime
        unsigned int curTestVal;
    public:
        //pre: 2 <= curVal <= maxVal
        PrimeAdder(int m_maxVal, int m_curVal, uint16_t m_parentId): Program(numInstances++), parentId(m_parentId), sum(0), maxVal(m_maxVal), curVal(m_curVal), curTestVal(1) {}
        PrimeAdder(int m_maxVal, int m_curVal): Program(numInstances++), parentId(0), sum(0), maxVal(m_maxVal), curVal(m_curVal), curTestVal(1) {}
        void execute(byte numCycles);
        void execute();
};

//anything with id 8 or greater is a primeAdder
//ids 0-7 are reserved for potential kernel processes
uint16_t PrimeAdder::numInstances = 8;

#endif