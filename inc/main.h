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
#include "serialPrintResult.h"

/*------------------------------------------
 Macros - Pins (Pin Defs to change according to Board Design)
------------------------------------------*/
#define MASTER_ENABLE   D5
#define PIN_SPI_SCLK    D13
#define PIN_SPI_CS      D10
#define PIN_SPI_MOSI    D11
#define PIN_SPI_MISO    D12
#define SPI_BITS        8
#define SPI_MODE        0 // CPOL - CPHA = 0 (0 - 0 = 0)

#define PIN_DRIVING     D14
#define PIN_CHARGING    D15
#define PIN_FAULT       D0
#define PIN_SHUTDOWN    PG_11 

/*------------------------------------------
 Macros - Communication Rates
------------------------------------------*/
#define SPI_CLK                500000
#define BAUD_RATE              115200
#define CAN_BAUD_RATE_DRIVE    250000
#define CAN_BAUD_RATE_CHARGE   250000

/*------------------------------------------
 Macros - CAN Msg IDs
------------------------------------------*/
#define CAN_CELL_V_ID   0x36
#define CAN_CHARGER_MSG_ID 0x1806E5F4


/*------------------------------------------
 Macros - Other
------------------------------------------*/
#define NUM_MODULES 1
#define NUM_CELL_PER_MODULE 12
#define NUM_CELLS  NUM_CELL_PER_MODULE*NUM_MODULES
#define TOTAL_IC NUM_MODULES

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

bool is_shutdown_closed();

void assert_fault_high();

void assert_fault_low();

void voltage_can_message(cell_asic *IC);

// void get_cell_voltages(uint8_t tIC, cell_asic * IC, float ** data);

// void read_cell_voltages(uint8_t tIC, cell_asic *ic, float ** data);

// void config_reg_init();

// End safe guards
#endif /* MAIN_H */
