#ifndef MESSAGE_H
#define MESSAGE_H

#include<Arduino.h>
#include "kernel.h"

//can dedicate two gpio ports to indicate when all devices are ready to
//recieve input. Each microcontroller will output a low signal when both
// it and the microcontroller it is connected to indicates that it is 
// ready to recieve input
//This same method can be used to indicate when a message is ready to be sent
#define MI 27//message interrupt
#define MB 26//message bus
#define MCLKO 25//message clock output
#define MCLKI 33//message clock input

void IRAM_ATTR message_send_isr();
void IRAM_ATTR message_receive_isr();
void IRAM_ATTR message_interrupt_isr();
class Message {
    private:
        static const uint8_t mclk_channel = 0;
        static constexpr double mclk_frequency = 10000000;
        static const uint8_t mclk_resolution = 2;

        uint8_t targetDevice;
        uint16_t targetProcess;
        uint32_t message;
        uint64_t compactedMsg;

    public:
        Message(uint8_t m_targetDevice, uint16_t m_targetProcess, uint32_t m_message): targetDevice(m_targetDevice), targetProcess(m_targetProcess), message(m_message) {
            compactedMsg = 0;
            compactedMsg |= message;
            compactedMsg <<= 16;
            compactedMsg |= targetProcess;
            compactedMsg <<= 8;
            compactedMsg |= targetDevice;
            compactedMsg <<= 3;
            compactedMsg |= 0b001;
        }
        ~Message();
        static void init();
        static void startClk();
        static void stopClk();
        static void sendMode();
        static void resolveSend();
        void send() const;
        static void handleMessage();
        static bool msgBusUsed;
        static bool msgSent;
        static uint8_t m_buffer[64];
        static uint8_t m_bufferIndex;
        static uint8_t m_bufferFilled;
        static uint64_t msgSending;
};

#endif