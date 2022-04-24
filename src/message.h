//The Message class handles all communication between devices.

#ifndef MESSAGE_H
#define MESSAGE_H

#include<Arduino.h>
#include <esp_now.h>
#include <WiFi.h>



struct Message {
    /*-------------Variables for all messages/message management--------------*/
    static const uint8_t broadcastAddress[6];//mac address of other device
    static esp_now_peer_info_t peerInfo;//information necessary to send messages
    static Message incomingMessage;//most recent recieved message
    static bool messageReceived;//whether a message has been recieved or not
    
    /*-----------------Member variables--------------------------------*/
    uint8_t targetProcess;
    uint8_t sendingProcess;
    uint32_t message;
    Message() {};
    Message(const uint8_t & m_targetProcess, const uint8_t & m_sendingProcess, const uint32_t & m_message): targetProcess(m_targetProcess), sendingProcess(m_sendingProcess), message(m_message) {}
    Message(const Message & message): targetProcess(message.targetProcess), sendingProcess(message.sendingProcess), message(message.message) {}
    
    //descr: used for initializing everything needed to send messages
    //pre: none
    //post: can send messages as needed by constructing and using Message::send()
    static void init();
    
    //descr: Used to send message
    //pre: must be a none-empty message ie member variables set.
    //post: message sent or failed to send to other device (targeted by broadcastAddress)
    void send();
};



// Callback when data is sent
void IRAM_ATTR OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

// Callback when data is received
void IRAM_ATTR OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

#endif