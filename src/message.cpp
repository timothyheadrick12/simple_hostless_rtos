#include "message.h"

bool Message::msgBusUsed = false;
bool Message::msgSent = false;
uint64_t Message::m_receiveBuffer[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};
uint8_t Message::m_bufferIndex = 0;
uint8_t Message::m_nextBufferIndex = 1;
uint8_t Message::m_bufferFilled = 0;
uint64_t Message::msgSending = 0;

void IRAM_ATTR message_send_isr() {
    digitalWrite(MB, Message::msgSending & (0x1 << (47 - Message::m_bufferIndex++)));
    if(Message::m_bufferIndex == 48) {
        detachInterrupt(MCLKI);
        Message::stopClk();
        Message::msgSent = true;
    }
}

void IRAM_ATTR message_receive_isr() {
    Message::m_receiveBuffer[Message::m_bufferIndex] |= digitalRead(MB);
    Message::m_receiveBuffer[Message::m_bufferIndex] << 1;
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

void Message::send() const{
    while(msgBusUsed);
    Message::sendMode();
    msgSending = compactedMsg;
    Message::startClk();
}

void Message::handleMessage() {
    uint8_t i = 0;
    while(!m_receiveBuffer[i])
        i++;
    while(!(m_receiveBuffer[i] & 0x1))
        m_receiveBuffer[i] >>= 1;
    if((m_receiveBuffer[i] >> 1) & 0x3) {
        Serial.println("Problem receiving a message");
        return;
    }
    m_receiveBuffer[i] >>= 3;
    uint8_t targetDevice = 0;
    targetDevice |= (m_receiveBuffer[i] & 0xFF);
    m_receiveBuffer[i] >>= 8;
    if(targetDevice != Kernel::id && targetDevice != 0xFF)
        return;
    uint16_t targetProcess = 0;
    targetProcess |= (m_receiveBuffer[i] & 0xFF);
    m_receiveBuffer[i] >>= 8;
    uint32_t message = 0;
    message |= (m_receiveBuffer[i] & 0xFFFFFFFF);
    m_receiveBuffer[i] = 0;
    Message::m_bufferFilled--;
    return;
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