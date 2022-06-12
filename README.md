# simple_hostless_rtos

This is an operating system I made to try to create the framework of an IoT network with no host device. The necessity of having some central hub that devices need to interact with in IoT networks has always seemed counter intuitive to me, so I wanted to work on a small scale project specifically meant to avoid that. Multiple ESP32s can connect to each other, and depending on how many ESP32s are connected, a prime adder program is split across the network so that the program can finish faster than with only one device executing it.

Here is a link to a full presentation on this os: https://docs.google.com/presentation/d/1kCtnTlxLXikblRP0pePS2LQ3wxsEMXw0iDW18_E6tF0/edit?usp=sharing

## The ESP32s in action:

<img src="https://user-images.githubusercontent.com/82300753/173242726-69ad0aa6-141d-437f-bbc9-74f7875431c5.jpg" alt="breadboard with two connected esp32s" width="200"/>

## Some Terminal Output:

<img src="https://user-images.githubusercontent.com/82300753/173242946-12a0f181-4b12-4049-a2c9-8a76d41e6b4e.png" alt="terminal output" width="300"/>
