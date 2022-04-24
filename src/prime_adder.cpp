#include "prime_adder.h"

uint8_t PrimeAdder::nextId = 8;

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal, const uint8_t & m_parentDeviceId,const uint8_t &  m_parentProcessId): Program(nextId++), parentDeviceId(m_parentDeviceId), parentProcessId(m_parentProcessId), numChildProcesses(0),numResponses(0),sum(0), maxVal(m_maxVal), curVal(m_curVal), curTestVal(1) {}

PrimeAdder::PrimeAdder(const uint16_t & m_maxVal, const uint16_t & m_curVal): Program(nextId++), parentProcessId(0), numResponses(0), sum(2) {
    processStartTime = millis();//set process start time
    maxVal = m_maxVal;//given maxval will be used by parent process
    curTestVal = 2;//curTestVal should start as 2

    //get the number of connected devices to decide if child processes should be generated
    uint8_t numConnectedDevices = Kernel::connectedDevices;
    
    //If no connected devices
    if(numConnectedDevices == 0) {
        curVal = m_curVal;
        numChildProcesses = 0;
        Serial.println("Created primeAdder on one device");
    } else {//at least one connected device
        Message* messageContainer;//to contain messages to be sent to each device
        uint32_t message = 0;//actual value of message
        numChildProcesses = numConnectedDevices;//A child process will be generated for each connected device
        uint16_t curMaxVal;//whatever maxval will be sent to the child process
        //for each connected device
        for(uint8_t i = 0; i < numConnectedDevices; i++) {
            curMaxVal = maxVal - (maxVal/(numConnectedDevices + 1)) * (numChildProcesses);
            if(i == 0) {
                curVal = maxVal - curMaxVal;//only set curVal for parent process during first iteration of for loop
            }

            /*---------- build message contents for child process-----------*/
            message |= curMaxVal;
            message <<= 16;
            message |= curMaxVal - (m_maxVal - m_curVal)/(numConnectedDevices + 1);
            messageContainer = new Message(CREATE_CHILD_PRIME_ADDER, id, message);
            messageContainer->send();
            numChildProcesses++;
        }
        numChildProcesses--;//there will be one more child process than actually exists at this point
        Serial.println("Created primeAdder on multiple devices");
    }

}

void PrimeAdder::execute(const double & numCycles) {
    uint8_t i = 0;
    //execute numCycles or until complete
    while(i < numCycles && !complete) {
        execute();
        i++;
    }
}

void PrimeAdder::execute() {
    //if this portion of process is complete
    if(curVal > maxVal) {
        //if waiting on child process, enter WAITING
        if(numChildProcesses > numResponses) {
            status = WAITING;
        }
        //process is complete
        else{
            complete = true;
            unsigned long completionTime = millis();
            Serial.println("-----------------------------");
            Serial.print("Finished prime addition: ");
            Serial.println(sum);
            Serial.print("In ");
            Serial.print((completionTime - processStartTime)/1000.0);
            Serial.println(" seconds.");
            Serial.println("-----------------------------");
            
            //Give result to parent if it is a child process
            if(parentProcessId) {
                Message(parentProcessId, id, sum).send();
            }
        }
    }

    //curVal is not a prime number if this is true, so move on to next curVal
    if(curVal % curTestVal == 0) {
        curVal++;
        curTestVal = 2;
    } else {//curVal may still be prime number, increment curTestVal to continue testing
        curTestVal++;
        //curVal is a prime number, add to sum and increment to next curVal
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
    //make sure status is READY if it wasn't already
    if(numChildProcesses == numResponses) {
        status = READY;
    }
}
