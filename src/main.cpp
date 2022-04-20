#include <Arduino.h>
#include "kernel.h"

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
}

void IRAM_ATTR button3_pressed_isr() {
    Serial.println("Button 3 pressed!");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_1, INPUT);
  attachInterrupt(BUTTON_1, button1_pressed_isr, FALLING);
  pinMode(BUTTON_2, INPUT);
  attachInterrupt(BUTTON_2, button2_pressed_isr, RISING);
  pinMode(BUTTON_3, INPUT);
  attachInterrupt(BUTTON_3, button3_pressed_isr, RISING);


}

void loop() {
//   put your main code here, to run repeatedly:
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
    
  if(Message::msgSent) {
    Message::resolveSend();
  }
  if(Message::m_bufferFilled) {
    kernel.handleMessage();
  }

}