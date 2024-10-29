#include "adbms_main.h"

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
 Macros - Pins
------------------------------------------*/
#define PIN_CLK         D13
#define PIN_CS          D10
#define PIN_MOSI        D11
#define PIN_MISO        D12
#define MASTER_ENABLE   D5

/*------------------------------------------
 Macros - Communication Rates
------------------------------------------*/
#define SPI_CLK         2000000
#define BAUD_RATE       115200

/*------------------------------------------
 States
------------------------------------------*/
typedef struct state {
    INIT,
    PRECHARGE,
    DRIVE_MAIN,
    DRIVE_DEBUG,
    CHARGE,
    FAULT
} state_t;

// Prototypes
void SetPinModes();

void spi_init(void);
