#include "kernel.h"
#include "message.h"
#include "prime_adder.h"

boolean Kernel::ledStatus = OFF;
uint8_t Kernel::connectedDevices = 0;

Kernel::Kernel() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
}

void Kernel::handleMessage() {
    if(Message::messageReceived) {
        Serial.println("Incoming Message!");
        Message message = Message(Message::incomingMessage);
        Message::messageReceived = false;
        switch(message.targetProcess) {
            case NEW_DEVICE_CONNECTED:
                connectedDevices++;
                Serial.println("Successfully recieved device connected message!");
                break;
            case TOGGLE_LED:
                toggleLed();
                break;
            case CREATE_CHILD_PRIME_ADDER:
                scheduler.push(new PrimeAdder(message.message >> 16, message.message & 0xFF, 0, message.sendingProcess));
                break;
            default:
                scheduler.find(message.targetProcess)->handleResponse(message.message);
                break;
        }
    }
}

void Kernel::toggleLed() {
    if(ledStatus) {
        digitalWrite(LED, LOW);
        ledStatus = OFF;
    } else {
        digitalWrite(LED, HIGH);
        ledStatus = ON;
    }
}
