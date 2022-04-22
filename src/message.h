#ifndef MESSAGE_H
#define MESSAGE_H

#include<Arduino.h>
#include <esp_now.h>
#include <WiFi.h>



struct Message {
    static const uint8_t broadcastAddress[6];
    static esp_now_peer_info_t peerInfo;
    static Message incomingMessage;
    static bool messageReceived;
    uint8_t targetProcess;
    uint8_t sendingProcess;
    uint32_t message;
    Message() {};
    Message(const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess, const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message) {}
    Message(const Message & message): targetProcess(message.targetProcess), sendingProcess(message.sendingProcess), message(message.message) {}
    static void init();
    void send();
};



// Callback when data is sent
void IRAM_ATTR OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

// Callback when data is received
void IRAM_ATTR OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

#endif