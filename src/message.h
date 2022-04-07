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
#define MIO 36//message interrupt output
#define MII 22//message interrupt input


class Message {
    private:
        byte targetDevice;
        uint16_t targetProcess;
        byte * message;
    public:
        Message(byte m_targetDevice, uint16_t m_targetProcess, byte * m_message): targetDevice(m_targetDevice), targetProcess(m_targetProcess), message(m_message) {}
        ~Message();
        byte* send() const;
        static byte* receive();
};

#endif