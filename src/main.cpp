#include <Arduino.h>
#include "message.h"
#include "scheduler.h"

#define LED 2
#define SPI_CLK_SPEED 10000000


byte deviceNum; 
byte numDevices = 1;
Kernel kernel;
Scheduler scheduler;
Program* curProgram = nullptr;
uint8_t i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  //Kernel kernel();
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
    curProgram = scheduler.getNext(i++);
  } while(i < schedulerLevels && !curProgram);
  curProgram->execute(pow(2, i-1));
  if(!curProgram->isComplete()) {
    scheduler.push(curProgram, i == schedulerLevels? i - 1 : i);
  }
  if(Message::msgSent) {
    Message::resolveSend();
  }
  if(Message::m_bufferFilled) {
    kernel.handleMessage();
  }

}