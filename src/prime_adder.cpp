#include "prime_adder.h"

//anything with id 8 or greater is a primeAdder
//ids 0-7 are reserved for potential kernel processes
uint8_t PrimeAdder::nextId = 8;

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const uint8_t &  m_parentId): Program(nextId++), parentId(m_parentId), sum(0), maxVal(m_maxVal), curVal(m_curVal), curTestVal(1) {

}

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const Kernel & kernel): Program(nextId++), parentId(0), sum(0) {
    maxVal = m_maxVal;
    curTestVal = 1;
    uint8_t numConnectedDevices = kernel.getConnectedDevices();
    if(numConnectedDevices == 0) {
        curVal = m_curVal;
        Serial.println("Created primeAdder on one device");
    } else {
        Message* messageContainer;
        uint32_t message;
        for(uint8_t i = 0; i < numConnectedDevices; i++) {
            if(i != Kernel::id) {
                message = m_maxVal / ;

                messageContainer = new Message(i, CREATE_CHILD_PRIME_ADDER, )
            }
        }
        Serial.println("Created primeAdder on multiple devices");
    }

}

void PrimeAdder::execute(const double & numCycles) {
    byte i = 0;
    while(i < numCycles && !complete) {
        execute();
    }
}

void PrimeAdder::execute() {
    if(curVal % curTestVal == 0) {
        curVal++;
        curTestVal = 2;
    } else {
        curTestVal++;
        if(curTestVal > curVal / 2) {
            sum += curVal;
            curVal++;
            curTestVal = 2;
        }
    }
    if(curVal > maxVal) {
        complete = true;
    }
}
