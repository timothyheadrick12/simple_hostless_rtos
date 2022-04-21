#ifndef MESSAGE_H
#define MESSAGE_H

#include<Arduino.h>

//Need to fix construct Message function to use pointers

//can dedicate two gpio ports to indicate when all devices are ready to
//recieve input. Each microcontroller will output a low signal when both
// it and the microcontroller it is connected to indicates that it is 
// ready to recieve input
//This same method can be used to indicate when a message is ready to be sent
#define MI 27//message interrupt
#define RXD2 16//RX input
#define TXD2 17//TX output

void IRAM_ATTR message_interrupt_isr();
class Message {
    private:
        uint8_t targetSendingDevice;
        uint8_t targetProcess;
        uint8_t sendingProcess;
        uint32_t message;

    public:
        Message(const uint8_t & m_targetDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message);
        Message(const uint8_t & m_targetDevice, const uint8_t & m_sendingDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message);
        static void init();
        void send();
        static void constructMessage(Message* messagePtr);
        static bool msgBusUsed;
        uint8_t getTargetProcess() const {return targetProcess;}
        uint8_t getSendingProcess() const {return sendingProcess;}
        uint8_t getMessage() const {return message;}
        uint8_t getSendingDevice() const {return targetSendingDevice >> 4;}
        uint8_t getTargetDevice() const {return targetSendingDevice & 0xF;}

};

#endif