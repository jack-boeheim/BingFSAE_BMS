#include "main.h"

/*-----------------------------------------------------------------------------
 Initialize SPI communication protocol
-----------------------------------------------------------------------------*/
void spi_init() {
    // Enable SPI with master enable and CS
    master = 1;
    chip_select = adBmsCsHigh();

    // 8-bit data frame, CPOL - CPHA = 0
    spi.format(SPI_BITS, SPI_MODE);

    // SPI clock of 2MHz
    spi.frequency(SPI_CLK);

    // USB baud rate
    // pc.baud(BAUD_RATE);
}

/*-----------------------------------------------------------------------------
 Initialize CAN communication protocol
-----------------------------------------------------------------------------*/
void can_init() {
    
}

/*-----------------------------------------------------------------------------
 Check the vehicle is driving (using the battery)
-----------------------------------------------------------------------------*/
bool is_driving() {
    return driving.read(PIN_DRIVING);
}

/*-----------------------------------------------------------------------------
 Check the vehicle is charging
-----------------------------------------------------------------------------*/
bool is_charging() {
    return charging.read(PIN_CHARGING);
}
