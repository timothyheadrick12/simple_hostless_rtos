#include <Arduino.h>
#include "message.h"

#define LED 2
#define SPI_CLK_SPEED 10000000


byte deviceNum; 
byte numDevices = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  //Kernel kernel();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);
  if(Message::msgSent) {
    Message::resolveSend();
  }
  if(Message::m_bufferFilled) {
    Message::handleMessage();
  }

}