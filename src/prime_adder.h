#ifndef PRIME_ADDER_H
#define PRIME_ADDER_H

#include <program.h>

class PrimeAdder: public Program {
    private:
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
        PrimeAdder(int max, int cur): sum(0), maxVal(max), curVal(cur) {}
        void execute(byte numCycles);
        void execute();
};

#endif