#include "message.h"
#include "kernel.h"

bool Message::msgBusUsed = false;
bool Message::msgSent = false;
uint64_t Message::m_receiveBuffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};
uint8_t Message::m_bufferIndex = 0;
uint8_t Message::m_bufferFilled = 0;
uint64_t Message::msgSending = 0;

void IRAM_ATTR message_send_isr() {
    digitalWrite(MB, Message::msgSending & (0x1 << (58 - Message::m_bufferIndex++)));
    if(Message::m_bufferIndex == 59) {
        Message::stopClk();
        detachInterrupt(MCLKI);
        Message::msgSent = true;
    }
}

void IRAM_ATTR message_receive_isr() {
    Message::m_receiveBuffer[Message::m_bufferIndex] |= digitalRead(MB);
    Message::m_receiveBuffer[Message::m_bufferIndex] <<= 1;
}

void IRAM_ATTR message_interrupt_isr() {
    if(digitalRead(MI)) //if rising
        Message::msgBusUsed = true;
    else { //if falling
        Message::msgBusUsed = false;
        Message::m_bufferIndex = Message::m_bufferIndex == 7? 0: Message::m_bufferIndex+1;
        Message::m_bufferFilled++;
    }
}

Message::Message(const uint8_t & m_targetDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message), compactedMsg(0) {
    targetSendingDevice = 0 | Kernel::id;
    targetSendingDevice <<= 4;
    targetSendingDevice |= m_targetDevice;
}

Message::Message(const uint8_t & m_targetDevice, const uint8_t & m_sendingDevice, const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess,const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message), compactedMsg(0) {
            targetSendingDevice = 0 | m_sendingDevice;
            targetSendingDevice <<= 4;
            targetSendingDevice |= m_targetDevice;
        }

void Message::send(){
    if(!compactedMsg) {
        compact();
    }
    while(msgBusUsed);
    Message::sendMode();
    msgSending = compactedMsg;
    Message::startClk();
}

void Message::constructMessage(Message * messagePtr) {
    if(Message::m_bufferFilled == 8){
        Serial.println("WARNING: The message buffer is full!");
    }
    uint8_t i = 0;
    while(!m_receiveBuffer[i])
        i++;
    while(!(m_receiveBuffer[i] & 0x1))
        m_receiveBuffer[i] >>= 1;
    if((m_receiveBuffer[i] >> 1) & 0x3) {
        Serial.println("ERROR: Problem receiving a message");
        messagePtr = nullptr;
        return;
    }
    m_receiveBuffer[i] >>= 3;
    uint8_t targetDevice = 0;
    targetDevice |= (m_receiveBuffer[i] & 0xF);
    m_receiveBuffer[i] >>= 4;
    if(targetDevice != Kernel::id && targetDevice != 0xF){
        messagePtr = nullptr;
        return;
    }
    uint8_t sendingDevice = 0;
    sendingDevice |= (m_receiveBuffer[i] & 0xF);
    m_receiveBuffer[i] >>= 4;
    uint8_t targetProcess = 0;
    targetProcess |= (m_receiveBuffer[i] & 0xFF);
    m_receiveBuffer[i] >>= 8;
    uint8_t sendingProcess = 0;
    sendingProcess |= (m_receiveBuffer[i] & 0xFF);
    m_receiveBuffer[i] >>= 8;
    uint32_t message = 0;
    message |= (m_receiveBuffer[i] & 0xFFFFFFFF);
    m_receiveBuffer[i] = 0;
    Message::m_bufferFilled--;
    messagePtr = new Message(targetDevice, sendingDevice, targetProcess, sendingProcess, message);
}

void Message::init() {
    pinMode(MI, INPUT);
    attachInterrupt(MI, message_interrupt_isr, CHANGE);
    pinMode(MB, INPUT);
    ledcSetup(mclk_channel, mclk_frequency, mclk_resolution);
    ledcWrite(mclk_channel, 0);
    ledcAttachPin(MCLKO, mclk_channel);
    pinMode(MCLKI, INPUT);
    attachInterrupt(MCLKI, message_receive_isr, FALLING);
}

void Message::startClk() {
    ledcWrite(mclk_channel, 2);
}

void Message::stopClk() {
    ledcWrite(mclk_channel, 0);
}

void Message::sendMode() {
    detachInterrupt(MI);
    pinMode(MI, OUTPUT);
    digitalWrite(MI, HIGH);
    detachInterrupt(MCLKI);
    attachInterrupt(MCLKI, message_send_isr, RISING);
    pinMode(MB, OUTPUT);
}

void Message::resolveSend() {
    pinMode(MI, INPUT);
    attachInterrupt(MI, message_interrupt_isr, CHANGE);
    pinMode(MB, INPUT);
    attachInterrupt(MCLKI, message_receive_isr, FALLING);
    msgSent = false;
    msgSending = 0;
    m_bufferIndex = 0;
}

void Message::compact(){
    compactedMsg = 0;
    compactedMsg |= message;
    compactedMsg <<= 8;
    compactedMsg |= sendingProcess;
    compactedMsg <<= 8;
    compactedMsg |= targetProcess;
    compactedMsg <<= 8;
    compactedMsg |= targetSendingDevice;
    compactedMsg <<= 3;
    compactedMsg |= 0b001;
}