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
extern DigitalIn shutdown_tap;
extern DigitalOut fault;



/*-----------------------------------------------------------------------------
 Initialize SPI communication protocol
-----------------------------------------------------------------------------*/
void spi_init() {
    // Enable SPI with master enable and CS
    master = 1;
    chip_select = 1;

    // 8-bit data frame, CPOL - CPHA = 0
    spi.format(SPI_BITS, SPI_MODE);

    // SPI clock of 500 kHz
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

bool is_shutdown_closed(){
    return shutdown_tap.read();
}

void assert_fault_high(){
    fault = 1;
}

void assert_fault_low(){
    fault = 0;
}

void voltage_can_message(cell_asic *IC) {
   CANMessage can_v_msg;
   can_v_msg.id = CAN_CELL_V_ID;    // Set CAN ID 
   can_v_msg.format = CANStandard;  // Use standard CAN format (11-bit ID)
   can_v_msg.type = CANData;        // Message type (data frame)
   can_v_msg.len = 8;               // Set length to 8 bytes
    for (int i = 0; i < NUM_MODULES;  ++i){
        for(int j = 0; j < NUM_CELLS; ++j){

                can_v_msg.data[0] = 12*i + j; //Cell ID
                can_v_msg.data[1] = ((IC[i].cell.c_codes[j] >> 8) & 0xFF); //8 MSB of Cell Voltage 
                can_v_msg.data[2] = (IC[i].cell.c_codes[j] & 0xFF); //8 LSB of Cell Voltage 
                can_v_msg.data[3] = 0; //8 LSB of Cell Voltage 
                can_v_msg.data[4] = 0; //8 LSB of Cell Voltage 
                can_v_msg.data[5] = 0; //8 LSB of Cell Voltage 
                can_v_msg.data[6] = 0; //8 LSB of Cell Voltage 
                can_v_msg.data[7] = 0; //Checksum 

        }       
    }
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