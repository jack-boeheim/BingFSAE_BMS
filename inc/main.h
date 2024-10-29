// Safe guards
#ifndef MAIN_H
#define MAIN_H

/*
    https://os.mbed.com/platforms/SDP_K1/
    Mbed SDP-K1 board MOSI, MOSI, SCK definition.
    Arduino Header Pinout
    MOSI = D11
    MISO = D12
    SCK  = D13
    CSB =  D10
*/

/*------------------------------------------
 Libraries
------------------------------------------*/
#include "adbms_main.h"

/*------------------------------------------
 Macros - Pins
------------------------------------------*/
#define MASTER_ENABLE   D5

#define PIN_SPI_SCLK    D13
#define PIN_SPI_CS      D10
#define PIN_SPI_MOSI    D11
#define PIN_SPI_MISO    D12
#define SPI_BITS        8
#define SPI_MODE        0 // CPOL - CPHA = 0 (0 - 0 = 0)

#define PIN_DRIVING     0
#define PIN_CHARGING    0

/*------------------------------------------
 Macros - Communication Rates
------------------------------------------*/
#define SPI_CLK         2000000
#define BAUD_RATE       115200

/*------------------------------------------
 States
------------------------------------------*/
typedef enum state {
    INIT,
    PRECHARGE,
    DRIVE_MAIN,
    DRIVE_DEBUG,
    CHARGE,
    FAULT
} state_t;

/*------------------------------------------
 Prototypes
------------------------------------------*/
void spi_init();

void can_init();

bool is_driving();

bool is_charging();

// End safe guards
#endif /* MAIN_H */
