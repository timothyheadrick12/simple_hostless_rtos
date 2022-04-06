#ifndef MESSAGE_H
#define MESSAGE_H

#include<Arduino.h>

enum MessageType : byte {
    primeAdder,
    ledControl,
};

//can dedicate two gpio ports to indicate when all devices are ready to
//recieve input. Each microcontroller will output a low signal when both
// it and the microcontroller it is connected to indicates that it is 
// ready to recieve input
//This same method can be used to indicate when a message is ready to be sent

class Message {
    private:
        byte target;
        MessageType type;
        byte * message;
    public:

};

#endif