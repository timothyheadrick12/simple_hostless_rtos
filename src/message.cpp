#include "message.h"
#include "kernel.h"
//-----------------------------------------------------Device 1---------------------------------Device 0
const uint8_t Message::broadcastAddress[6] = /*{0xC8, 0xC9, 0xA3, 0xC5, 0xC8, 0x38};*/ {0xC8, 0xC9, 0xA3, 0xC5, 0xDE, 0x94};
esp_now_peer_info_t Message::peerInfo;
Message Message::incomingMessage;
bool Message::messageReceived = false;

void IRAM_ATTR OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void IRAM_ATTR OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Message::incomingMessage, incomingData, sizeof(Message::incomingMessage));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Message::messageReceived = true;
}

void Message::send() {
    esp_now_send(broadcastAddress, (uint8_t *) &(*this), sizeof(*this));
}

void Message::init() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    } else 
        Kernel::connectedDevices = 1;
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);
}