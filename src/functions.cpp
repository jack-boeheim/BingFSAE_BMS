#include "main.h"
#include "adBms_Application.h"
#include "serialPrintResult.h"
#include "mcuWrapper.h"




extern SPI spi; 
extern CAN can;
extern Serial pc;
extern cell_asic IC;
extern DigitalOut master;
extern DigitalOut chip_select;
extern DigitalIn driving;
extern DigitalIn charging;


// /* ADC Command Configurations */
// RD      REDUNDANT_MEASUREMENT           = RD_OFF;
// CH      AUX_CH_TO_CONVERT               = AUX_ALL;
// CONT    CONTINUOUS_MEASUREMENT          = SINGLE;
// OW_C_S  CELL_OPEN_WIRE_DETECTION        = OW_OFF_ALL_CH;
// OW_AUX  AUX_OPEN_WIRE_DETECTION         = AUX_OW_OFF;
// PUP     OPEN_WIRE_CURRENT_SOURCE        = PUP_DOWN;
// DCP     DISCHARGE_PERMITTED             = DCP_OFF;
// RSTF    RESET_FILTER                    = RSTF_OFF;
// ERR     INJECT_ERR_SPI_READ             = WITHOUT_ERR;

// /* Set Under Voltage and Over Voltage Thresholds */
// const float OV_THRESHOLD = 4.2;                 /* Volt */
// const float UV_THRESHOLD = 3.0;                 /* Volt */
// const int OWC_Threshold = 2000;                 /* Cell Open wire threshold(mili volt) */
// const int OWA_Threshold = 50000;                /* Aux Open wire threshold(mili volt) */
// const uint32_t LOOP_MEASUREMENT_COUNT = 1;      /* Loop measurment count */
// const uint16_t MEASUREMENT_LOOP_TIME  = 10;     /* milliseconds(mS)*/
// uint32_t loop_count = 0;
// uint32_t pladc_count;

// /*Loop Measurement Setup These Variables are ENABLED or DISABLED Remember ALL CAPS*/
// LOOP_MEASURMENT MEASURE_CELL            = ENABLED;        /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_AVG_CELL        = ENABLED;        /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_F_CELL          = ENABLED;        /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_S_VOLTAGE       = ENABLED;        /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_AUX             = DISABLED;       /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_RAUX            = DISABLED;       /*   This is ENABLED or DISABLED       */
// LOOP_MEASURMENT MEASURE_STAT            = DISABLED;       /*   This is ENABLED or DISABLED       */

/*-----------------------------------------------------------------------------
 Initialize SPI communication protocol
-----------------------------------------------------------------------------*/
void spi_init() {
    // Enable SPI with master enable and CS
    master = 1;
    chip_select = 1;

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
    return driving.read();
}

/*-----------------------------------------------------------------------------
 Check the vehicle is charging
-----------------------------------------------------------------------------*/
bool is_charging() {
    return charging.read();
}

/*-----------------------------------------------------------------------------
 Get all BMS cell voltages stored in an array
-----------------------------------------------------------------------------*/
// void get_cell_voltages(uint8_t tIC, cell_asic * IC, float ** data) {
//     float voltage;
//     int16_t temp;
    
//     // 
//     for (uint8_t ic = 0; ic < tIC; ++ic) {
//         for (uint8_t index = 0; index < CELL; ++index) {
//             temp = IC[ic].cell.c_codes[index];
//             voltage = getVoltage(temp);

//             data[ic][index] = voltage;
//         }
//     }
    
// }

// /*-----------------------------------------------------------------------------
//  Update all cell voltages
// -----------------------------------------------------------------------------*/
// void read_cell_voltages(uint8_t tIC, cell_asic * ic, float ** data) {
//   adBmsWakeupIc(tIC);
//   adBmsReadData(tIC, &ic[0], RDCVA, Cell, A);
//   adBmsReadData(tIC, &ic[0], RDCVB, Cell, B);
//   adBmsReadData(tIC, &ic[0], RDCVC, Cell, C);
//   adBmsReadData(tIC, &ic[0], RDCVD, Cell, D);
//   adBmsReadData(tIC, &ic[0], RDCVE, Cell, E);
//   adBmsReadData(tIC, &ic[0], RDCVF, Cell, F);
//   data = get_cell_voltages(tIC, &ic[0], data);
// }

// /*-----------------------------------------------------------------------------
//  Initialize the config registers
// -----------------------------------------------------------------------------*/
// void config_reg_init() {
//     uint8_t loop_count = 0;

//     adBmsWakeupIc(NUM_MODULES);
//     adBmsWriteData(NUM_MODULES, &IC[0], WRCFGA, Config, A);
//     adBmsWriteData(NUM_MODULES, &IC[0], WRCFGB, Config, B);
//     adBmsWakeupIc(NUM_MODULES);
    
//     adBms6830_Adcv(REDUNDANT_MEASUREMENT, CONTINUOUS, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
//     Delay_ms(1);
//     adBms6830_Adcv(RD_ON, CONTINUOUS, DISCHARGE_PERMITTED, RESET_FILTER, CELL_OPEN_WIRE_DETECTION);
//     Delay_ms(1);
//     adBms6830_Adsv(CONTINUOUS, DISCHARGE_PERMITTED, CELL_OPEN_WIRE_DETECTION);
//     Delay_ms(8);

//     while(loop_count < LOOP_MEASUREMENT_COUNT) {
//       measurement_loop();
//       Delay_ms(MEASUREMENT_LOOP_TIME);
      
//       ++loop_count;
//     }
// }