#ifndef MAIN_H
#define MAIN_H


/*------------------------------------------
 Macros - Software Modes
------------------------------------------*/
// #define DEBUG Enable to use Serial Debug Statements 
// #define MBED5 Enable for use with MBED 5 OS 

/*------------------------------------------
 Libraries
------------------------------------------*/
#include "adbms_main.h"
#include "common.h"

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
#define PIN_SHUTDOWN    D1
#define PIN_FAN_0       D2
#define PIN_FAN_1       D3
#define PIN_FAN_2       D4
#define PIN_FAN_3       D5
#define PIN_FAN_4       D6
#define PIN_FAN_5       D7
#define PIN_FAN_6       D8
#define PIN_FAN_7       D9



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
#define NUM_FANS 1 
#define FAN_PWM_PERIOD_US 11627

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

void set_fault_high();

void set_fault_low();

void voltage_can_message(cell_asic *IC, CANMessage *can_v_msg);

bool check_OV_UV_flags(cell_asic *IC, uint16_t * pCellErrorBuf);

_Bool read_charger_can_message(float * pOutputVoltageV, float * pOutputCurrentA);

void set_fan_pwm(float PWM_speed, int PWM_period_us, int fan_select);

#endif /* MAIN_H */
