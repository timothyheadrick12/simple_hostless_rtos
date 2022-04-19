#include "kernel.h"
#include "message.h"
#include "prime_adder.h"
uint8_t Kernel::id = DEVICE_ID;
uint8_t Kernel::connectedDevices = 0;
boolean Kernel::ledStatus = OFF;

void IRAM_ATTR device_disconnected_isr() {
    Kernel::connectedDevices--;
}

Kernel::Kernel() {
    connectedDevices = 0;
    Message::init();
    pinMode(CONNECTED_DEVICE_I, INPUT);
    attachInterrupt(CONNECTED_DEVICE_I, device_disconnected_isr, FALLING);
    pinMode(CONNECTED_DEVICE_O, OUTPUT);
    pinMode(CONNECTED_DEVICE_O, HIGH);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    Message(0xF, 0x0, 0x0, id).send();
}

void Kernel::handleMessage() {
    Message * message = nullptr;
    Message::constructMessage(message);
    if(message != nullptr) {
        switch(message->getTargetProcess()) {
            case NEW_DEVICE_CONNECTED:
                connectedDevices++;
                break;
            case TOGGLE_LED:
                if(ledStatus) {
                    digitalWrite(LED, LOW);
                    ledStatus = OFF;
                } else {
                    digitalWrite(LED, HIGH);
                    ledStatus = ON;
                }
                break;
            case CREATE_CHILD_PRIME_ADDER:
                scheduler.push(new PrimeAdder(message->getMessage() >> 16, message->getMessage() & 0xFF, message->getSendingDevice(), message->getSendingProcess()));
                break;
            default:
                scheduler.find(message->getTargetProcess())->handleResponse(message->getMessage());
                break;
        }
    }
}
