#include "message.h"
#include "kernel.h"

bool Message::msgBusUsed = false;

void IRAM_ATTR message_interrupt_isr() {
    if(digitalRead(MI)) //if rising
        Message::msgBusUsed = true;
    else { //if falling
        Message::msgBusUsed = false;
    }
}

Message::Message(const uint8_t & m_targetDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message) {
    targetSendingDevice = 0 | Kernel::id;
    targetSendingDevice <<= 4;
    targetSendingDevice |= m_targetDevice;
}

Message::Message(const uint8_t & m_targetDevice, const uint8_t & m_sendingDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message) {
            targetSendingDevice = 0 | m_sendingDevice;
            targetSendingDevice <<= 4;
            targetSendingDevice |= m_targetDevice;
        }

void Message::send(){
    while(msgBusUsed);
    pinMode(MI, OUTPUT);
    digitalWrite(MI, HIGH);
    uint8_t messageBytes[7] = {targetSendingDevice, targetProcess, sendingProcess, message >> 48 & 0xFF, message >> 32 & 0xFF, message >> 16 * 0xFF, message & 0xFF};
    Serial2.write(messageBytes, 7);
    digitalWrite(MI, LOW);
    pinMode(MI, INPUT);
}

void Message::constructMessage(Message * messagePtr) {
    Serial.println("Constructing message!");
    uint8_t messageBytes[7];
    Serial2.readBytes(messageBytes, 7);
    uint8_t targetDevice = (messageBytes[0] & 0xF);
    messageBytes[0] >>= 4;
    if(targetDevice != Kernel::id && targetDevice != 0xF){
        messagePtr = nullptr;
        Serial.println("Problem with message!");
        return;
    }
    uint8_t sendingDevice = (messageBytes[0] & 0xF);
    uint8_t targetProcess = messageBytes[1];
    uint8_t sendingProcess = messageBytes[2];
    uint32_t message = 0;
    message |= messageBytes[3];
    message <<= 8;
    message |= messageBytes[4];
    message <<= 8;
    message |= messageBytes[5];
    message <<= 8;
    message |= messageBytes[6];
    messagePtr = new Message(targetDevice, sendingDevice, targetProcess, sendingProcess, message);
}

void Message::init() {
    pinMode(MI, INPUT);
    attachInterrupt(MI, message_interrupt_isr, CHANGE);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}