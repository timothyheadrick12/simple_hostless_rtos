#include <Arduino.h>
#include "kernel.h"

Kernel kernel;
Program* curProgram = nullptr;
uint8_t i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(LED, HIGH);
  // Serial.println("LED is on");
  // delay(1000);
  // digitalWrite(LED, LOW);
  // Serial.println("LED is off");
  // delay(1000);
  i = 0;
  do {
    curProgram = kernel.scheduler.getNext(i++);
  } while(i < schedulerLevels && !curProgram);
  curProgram->execute(pow(2, i-1));
  if(!curProgram->isComplete()) {
    kernel.scheduler.push(curProgram, i == schedulerLevels? i - 1 : i);
  }
  if(Message::msgSent) {
    Message::resolveSend();
  }
  if(Message::m_bufferFilled) {
    kernel.handleMessage();
  }

}