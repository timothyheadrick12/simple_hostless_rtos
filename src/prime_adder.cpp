#include "prime_adder.h"

//anything with id 8 or greater is a primeAdder
//ids 0-7 are reserved for potential kernel processes
uint8_t PrimeAdder::nextId = 8;

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const uint8_t & m_parentDeviceId,const uint8_t &  m_parentProcessId): Program(nextId++), parentDeviceId(m_parentDeviceId), parentProcessId(m_parentProcessId), numChildProcesses(0),numResponses(0),sum(0), maxVal(m_maxVal), curVal(m_curVal), curTestVal(1) {

}

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const Kernel & kernel): Program(nextId++), parentProcessId(0), numResponses(0), sum(0) {
    maxVal = m_maxVal;
    curTestVal = 1;
    uint8_t numConnectedDevices = kernel.getConnectedDevices();
    if(numConnectedDevices == 0) {
        curVal = m_curVal;
        Serial.println("Created primeAdder on one device");
    } else {
        Message* messageContainer;
        uint32_t message = 0;
        numChildProcesses = 1;
        uint16_t curMaxVal;
        for(uint8_t i = 0; i < numConnectedDevices; i++) {
            if(i != Kernel::id) {
                curMaxVal = maxVal - (maxVal/(numConnectedDevices + 1)) * (numChildProcesses);
                message |= curMaxVal;
                message <<= 16;
                message |= curMaxVal - (m_maxVal - m_curVal)/(numConnectedDevices + 1);
                messageContainer = new Message(i, CREATE_CHILD_PRIME_ADDER, id, message);
                messageContainer->send();
                numChildProcesses++;
            }
        }
        numChildProcesses--;
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
    if(curVal > maxVal) {
        if(numChildProcesses > numResponses) {
            status = WAITING;
        }
        else{
            complete = true;
            if(parentProcessId) {
                Message(parentDeviceId, parentProcessId, id, sum).send();
            }
        }
    }
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
}

void PrimeAdder::handleResponse(const uint32_t & response) {
    numResponses++;
    sum += response;
    if(numChildProcesses == numResponses) {
        status = READY;
    }
}
