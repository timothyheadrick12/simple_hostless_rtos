//Contains main loop, button interrupts, and initial setup. Main loop manages the scheduler and handling any incoming messages.

#include <Arduino.h>
#include <Wifi.h>
#include "kernel.h"
#include "prime_adder.h"
#include "led_toggler.h"


//Device 0 mac address: C8:C9:A3:C5:C8:38
//Device 1 mac address: C8:C9:A3:C5:DE:94

/*---------------------BUTTON PINS--------------------*/
#define BUTTON_1 0
#define BUTTON_2 5
#define BUTTON_3 18
#define BUTTON_4 17

Kernel kernel;//main kernel instance
Program* curProgram = nullptr;//pointer to whatever the current process being operated on is

/*--------------Set when corresponding button is pressed to schedule process---------*/
volatile boolean schedulePrimeAdder = false;
volatile boolean scheduleLedToggler = false;

/*---------------------------Used to handle debounce of buttons------------------*/
volatile unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

/*----------------------------Button interrupt service routines--------------*/
//This button scheduls a primeAdder
void IRAM_ATTR button1_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 1 pressed!");
        schedulePrimeAdder = true;
    }
    lastDebounceTime = millis();  
}

//This button toggles the on board LED
void IRAM_ATTR button2_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 2 pressed!");
        scheduleLedToggler = true;
    }
    lastDebounceTime = millis();  
}

//This button send a message to toggle the connected device LED
void IRAM_ATTR button3_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 3 pressed!");
        Message(TOGGLE_LED, 0, 0).send();
    }
    lastDebounceTime = millis();  
}

//This button changes how many devices are connected for primeAdder testing purposes
void IRAM_ATTR button4_pressed_isr() {
    if(millis() - lastDebounceTime > debounceDelay) {
        Serial.println("Button 4 pressed!");
        if(Kernel::connectedDevices == 1)
            Kernel::connectedDevices = 0;
        else
            Kernel::connectedDevices = 1;
    }
    lastDebounceTime = millis();  
}

void setup() {
    Serial.begin(115200);//Set for output to terminal
    //Button initializations
    pinMode(BUTTON_1, INPUT);
    attachInterrupt(BUTTON_1, button1_pressed_isr, FALLING);
    pinMode(BUTTON_2, INPUT);
    attachInterrupt(BUTTON_2, button2_pressed_isr, RISING);
    pinMode(BUTTON_3, INPUT);
    attachInterrupt(BUTTON_3, button3_pressed_isr, RISING);
    pinMode(BUTTON_4, INPUT);
    attachInterrupt(BUTTON_4, button4_pressed_isr, RISING);
    Message::init();//initialization needed for esp_now messages to work

}

void loop() {
    /*-------------------schedule process after corresponding button is pressed-----------------*/
    if(schedulePrimeAdder) {
        kernel.scheduler.push(new PrimeAdder(25000, 1));
        Serial.println("Scheduled Prime Adder!");
        schedulePrimeAdder = false;
    }
    if(scheduleLedToggler) {
        kernel.scheduler.push(new LedToggler());
        Serial.println("Scheduled ledToggler!");
        scheduleLedToggler = false;
    }
    uint8_t i = 0;//used to increment through scheduler levels
    do {
        //get first process in the given scheduler level
        //nullptr if no process found in given scheduler level
        //process is also removed from scheduler when returned
        curProgram = kernel.scheduler.getNext(i++);
    } while(i < schedulerLevels && !curProgram);
    //if process found
    if(curProgram) {
        //execute process based on scheduler level, implements a multilevel feedback approach
        curProgram->execute(pow(2, i-1));
        
        if(!(curProgram->isComplete())) {
            //if process not complete push it to the next level of the scheduler
            kernel.scheduler.push(curProgram, i == schedulerLevels? i - 1: i);
        } else {
            //if process is complete, delete it
            kernel.scheduler.remove(curProgram);
            Serial.print("Process ");
            Serial.print(curProgram->getId());
            Serial.println(" completed!");
        }
    }
    kernel.handleMessage();//handle any messages that have been recieved in the last loop

}