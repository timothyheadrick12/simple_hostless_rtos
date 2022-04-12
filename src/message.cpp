#include "message.h"

bool Message::msgBusUsed = false;
bool Message::msgSent = false;
uint8_t Message::m_buffer[64];
uint8_t Message::m_bufferIndex = 0;
uint8_t Message::m_bufferFilled = false;
uint64_t Message::msgSending = 0;

void IRAM_ATTR message_send_isr() {
    digitalWrite(MB, Message::msgSending & 0x1 << Message::m_bufferIndex++);
    if(Message::m_bufferIndex == 60) {
        detachInterrupt(MCLKI);
        Message::stopClk();
        Message::msgSent = true;
    }
}

void IRAM_ATTR message_receive_isr() {
    Message::m_buffer[Message::m_bufferIndex++] = digitalRead(MB);
}

void IRAM_ATTR message_interrupt_isr() {
    if(digitalRead(MI)) //if rising
        Message::msgBusUsed = true;
    else { //if falling
        Message::msgBusUsed = false;
        Message::m_bufferIndex = 0;
        Message::m_bufferFilled = true;
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
    while(!m_buffer[i])
        i++;
    if(m_buffer[i+1] || m_buffer[i+2]) {
        Serial.println("Problem receiving a message");
        return;
    }
    i += 3;
    uint8_t targetDevice = 0;
    for(uint8_t j = 0; j < 8; j++) {
        targetDevice |= m_buffer[i+j] << j;
    }
    if(targetDevice != Kernel::id)
        return;
    i += 8;
    uint16_t targetProcess = 0;
    for(uint8_t j = 0; j < 16; j++) {
        targetProcess |= m_buffer[i+j] << j;
    }
    i += 16;
    uint32_t message = 0;
    for(uint8_t j = 0; j < 32; j++) {
        message |= m_buffer[i+j] << j;
    }
    Message::m_bufferFilled = false;
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