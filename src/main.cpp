#include <Arduino.h>
#include <Wifi.h>
#include "kernel.h"
#include "prime_adder.h"


//Device 0 mac address: C8:C9:A3:C5:C8:38
//Device 1 mac address: C8:C9:A3:C5:DE:94

#define BUTTON_1 0
#define BUTTON_2 5
#define BUTTON_3 18

Kernel kernel;
Program* curProgram = nullptr;
volatile boolean schedulePrimeAdder = false;
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

void IRAM_ATTR button1_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 1 pressed!");
        schedulePrimeAdder = true;
    }
    lastDebounceTime = millis();  
}

void IRAM_ATTR button2_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 2 pressed!");
        kernel.toggleLed();
    }
    lastDebounceTime = millis();  
}

void IRAM_ATTR button3_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 3 pressed!");
        Message(TOGGLE_LED, 0, 0).send();
    }
    lastDebounceTime = millis();  
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_1, INPUT);
    attachInterrupt(BUTTON_1, button1_pressed_isr, FALLING);
    pinMode(BUTTON_2, INPUT);
    attachInterrupt(BUTTON_2, button2_pressed_isr, RISING);
    pinMode(BUTTON_3, INPUT);
    attachInterrupt(BUTTON_3, button3_pressed_isr, RISING);
    Message::init();

}

void loop() {
    if(schedulePrimeAdder) {
        kernel.scheduler.push(new PrimeAdder(10000, 1));
        Serial.println("Scheduled Prime Adder!");
        schedulePrimeAdder = false;
    }
    uint8_t i = 0;
    do {
        curProgram = kernel.scheduler.getNext(i++);
    } while(i < schedulerLevels && !curProgram);
    if(curProgram) {
        curProgram->execute(pow(2, i-1));
        if(!(curProgram->isComplete())) {
            kernel.scheduler.push(curProgram, i == schedulerLevels? i - 1: i);
        } else {
            kernel.scheduler.remove(curProgram);
            Serial.print("Process ");
            Serial.print(curProgram->getId());
            Serial.println(" completed!");
        }
    }
    kernel.handleMessage();

}