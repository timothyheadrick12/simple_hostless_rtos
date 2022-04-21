#include <Arduino.h>
#include "kernel.h"
#include <WiFi.h>

//Device 0 mac address: C8:C9:A3:C5:C8:38
//Device 1 mac address: C8:C9:A3:C5:DE:94

#define BUTTON_1 0
#define BUTTON_2 5
#define BUTTON_3 18

Kernel kernel;
Program* curProgram = nullptr;
uint8_t i = 0;

void IRAM_ATTR button1_pressed_isr() {
    Serial.println("Button 1 pressed!");
}

void IRAM_ATTR button2_pressed_isr() {
    Serial.println("Button 2 pressed!");
    Message(1, TOGGLE_LED, 0, 0);
}

void IRAM_ATTR button3_pressed_isr() {
    Serial.println("Button 3 pressed!");
    Message(0, TOGGLE_LED, 0, 0);
}

void setup() {
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  while(Serial2.available() > 0)
    char t = Serial2.read();
  pinMode(BUTTON_1, INPUT);
  attachInterrupt(BUTTON_1, button1_pressed_isr, FALLING);
  pinMode(BUTTON_2, INPUT);
  attachInterrupt(BUTTON_2, button2_pressed_isr, RISING);
  pinMode(BUTTON_3, INPUT);
  attachInterrupt(BUTTON_3, button3_pressed_isr, RISING);


}

void loop() {
  i = 0;
  do {
    curProgram = kernel.scheduler.getNext(i++);
  } while(i < schedulerLevels && !curProgram);
  if(curProgram) {
        curProgram->execute(pow(2, i-1));
    if(!curProgram->isComplete()) {
        kernel.scheduler.push(curProgram, i == schedulerLevels? i - 1 : i);
    }
  }
  if(Serial2.available() > 6) {
    Serial.println("Buffer Filled!");
    kernel.handleMessage();
  }

}