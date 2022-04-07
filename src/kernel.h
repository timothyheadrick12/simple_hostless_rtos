#ifndef KERNEL_H
#define KERNEL_H

#include <Arduino.h>
#include <ESP32SPISlave.h>
#include <SPI.h>

#define MIO 36//message interrupt output
#define MII 22//message interrupt input
#define SPI_CLK_SPEED 10000000

class Kernel {
    private:
        byte connectedDevices;
        byte id;
        ESP32SPISlave slave;
        SPIClass master;
        static const uint32_t BUFFER_SIZE {8};
        uint8_t spi_master_tx_buf[BUFFER_SIZE];
        uint8_t spi_master_rx_buf[BUFFER_SIZE];
        uint8_t spi_slave_tx_buf[BUFFER_SIZE];
        uint8_t spi_slave_rx_buf[BUFFER_SIZE];
    public:
        Kernel();
};

#endif