#include "kernel.h"

Kernel::Kernel(): connectedDevices(0), id(0), slave(), master(VSPI) {
    pinMode(MIO, OUTPUT);
    pinMode(MII, INPUT);
    master.begin();
    slave.setDataMode(SPI_MODE0);
    slave.begin(HSPI);
}