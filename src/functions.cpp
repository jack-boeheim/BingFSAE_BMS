#include "fsm.h"

/*-----------------------------------------------------------------------------
 Set pin modes
-----------------------------------------------------------------------------*/
void SetPinModes() {
    DigitalIn miso(PIN_MISO);                      /* MISO */

    DigitalOut master(MASTER_ENABLE);
    DigitalOut chip_select(PIN_CS);                /* SPI chip select */
    DigitalOut mosi(PIN_MOSI);                     /* MOSI */
    DigitalOut sclk(PIN_CLK);                      /* SCK */       
}

/*-----------------------------------------------------------------------------
 Initialize SPI communication protocol
-----------------------------------------------------------------------------*/
void spi_init() {   
    SPI spi(PIN_MOSI, PIN_MISO, PIN_CLK);                     /* SPI MOSI, MISO, SCK */
    
    master = 1;
    chip_select = adBmsCsHigh();
    spi.format(8,0);            /* 8bit data, CPOL-CPHA= 0*/
    spi.frequency(SPI_CLK);     /* SPI clock 2Mhz */

    // pc.baud(115200);         /* Usb baud rate */   
}