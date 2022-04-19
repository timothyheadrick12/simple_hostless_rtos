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
#define MB 26//message bus
#define MCLKO 25//message clock output
#define MCLKI 33//message clock input
#define BUFFER_SIZE 8

void IRAM_ATTR message_send_isr();
void IRAM_ATTR message_receive_isr();
void IRAM_ATTR message_interrupt_isr();
class Message {
    private:
        static const uint8_t mclk_channel = 0;
        static constexpr double mclk_frequency = 100000;
        static const uint8_t mclk_resolution = 2;

        uint8_t targetSendingDevice;
        uint8_t targetProcess;
        uint8_t sendingProcess;
        uint32_t message;
        uint64_t compactedMsg;

        void compact();

    public:
        Message(const uint8_t & m_targetDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message);
        Message(const uint8_t & m_targetDevice, const uint8_t & m_sendingDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message);
        static void init();
        static void startClk();
        static void stopClk();
        static void sendMode();
        static void resolveSend();
        void send();
        static void constructMessage(Message* messagePtr);
        static bool msgBusUsed;
        static bool msgSent;
        static uint64_t m_receiveBuffer[BUFFER_SIZE];
        static uint8_t m_bufferIndex;
        static uint8_t m_bufferFilled;
        static uint64_t msgSending;
        uint8_t getTargetProcess() const {return targetProcess;}
        uint8_t getSendingProcess() const {return sendingProcess;}
        uint8_t getMessage() const {return message;}
        uint8_t getSendingDevice() const {return targetSendingDevice >> 4;}
        uint8_t getTargetDevice() const {return targetSendingDevice & 0xF;}

};

#endif