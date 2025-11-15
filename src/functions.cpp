#include "main.h"
#include "adBms_Application.h"
#include "serialPrintResult.h"
#include "mcuWrapper.h"
#include <cstdint>

extern SPI spi; 
extern CAN can;
//extern Serial pc;
extern cell_asic IC;
extern DigitalOut master;
extern DigitalOut chip_select;
extern DigitalIn driving;
extern DigitalIn charging;
extern DigitalIn shutdown_tap;
extern DigitalOut fault;
extern Timer canTimer;


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
        for(int j = 0; j < NUM_CELLS_PER_MODULE; ++j){
                can_v_msg.data[0] = 12*i + j; //Cell ID
                can_v_msg.data[1] = ((IC[i].cell.c_codes[j] >> 8) & 0xFF); //8 MSB of InstantaneousCell Voltage 
                can_v_msg.data[2] = (IC[i].cell.c_codes[j] & 0xFF); //8 LSB of Instantaneous Cell Voltage 
                can_v_msg.data[3] = 0; //8 MSB of Internal Resistance  (placeholder 0 for now)
                can_v_msg.data[4] = 0; //8 LSB of Internal Resistance  (placeholder 0 for now)
                can_v_msg.data[5] = 0; //8 MSB of Open Circuit Voltage (placeholder 0 for now)
                can_v_msg.data[6] = 0; //8 LSB of Open Circuit Voltage (placeholder 0 for now)
                can_v_msg.data[7] = (((CAN_CELL_V_ID + 8 + can_v_msg.data[0] + can_v_msg.data[1] + 
                can_v_msg.data[2] + can_v_msg.data[3] + can_v_msg.data[4] + 
                can_v_msg.data[5] + can_v_msg.data[6]) >> 8) & 0xFF); //Checksum (used same process as Orion)
                can.write(can_v_msg);      
                Delay_ms(1);
        } 
       
    }
}

void temp_can_msg(cell_asic *IC) {

    // voltage = getVoltage(IC[ic].stata.itmp);
    // printf("ITMP:%f°C\n\n", (voltage/0.0075)-273);

   CANMessage can_temp_msg;
   can_temp_msg.id = CAN_CELL_TEMP_ID;    // Set CAN ID 
   can_temp_msg.format = CANStandard;  // Use standard CAN format (11-bit ID)
   can_temp_msg.type = CANData;        // Message type (data frame) 
   can_temp_msg.len = 8;               // Set length to 8 bytes
    for (int i = 0; i < NUM_MODULES;  ++i){
        for(int j = 0; j < NUM_CELLS_PER_MODULE; ++j){
                can_temp_msg.data[0] = 12*i + j; //Cell ID
                can_temp_msg.data[1] = ((IC[i].stata.itmp >> 8) & 0xFF); //8 MSB of temperature Voltage 
                can_temp_msg.data[2] = (IC[i].stata.itmp & 0xFF); //8 LSB of temperature Voltage 
                can_temp_msg.data[3] = 0; 
                can_temp_msg.data[4] = 0; 
                can_temp_msg.data[5] = 0;
                can_temp_msg.data[6] = 0;
                can_temp_msg.data[7] = (((CAN_CELL_V_ID + 8 + can_temp_msg.data[0] + can_temp_msg.data[1] + 
                can_temp_msg.data[2] + can_temp_msg.data[3] + can_temp_msg.data[4] + 
                can_temp_msg.data[5] + can_temp_msg.data[6]) >> 8) & 0xFF); //Checksum (used same process as Orion)
                can.write(can_temp_msg);      
                Delay_ms(1);
        } 
       
    }
}

void charger_can_message(uint16_t maxChargeV, uint16_t maxChargeI, bool bChargeSafe){
   CANMessage can_charge_msg;
   can_charge_msg.id = CAN_CHARGER_MSG_ID;  // Set CAN ID 
   can_charge_msg.format = CANExtended;     // Use CAN Extended format (29-bit ID) for Charger
   can_charge_msg.type = CANData;           // Message type (data frame) 
   can_charge_msg.len = 8;                  // Set length to 8 bytes
   
   //CAN Data Frame per Elcon Charger Specifications 
   can_charge_msg.data[0] = ((maxChargeV >> 8) & 0xFF); 
   can_charge_msg.data[1] = (maxChargeV & 0xFF); 
   can_charge_msg.data[2] = ((maxChargeI >> 8) & 0xFF); 
   can_charge_msg.data[3] = (maxChargeI & 0xFF); 
   can_charge_msg.data[4] = bChargeSafe;

   can.write(can_charge_msg);

}


_Bool read_charger_can_message(float * pOutputVoltageV, float * pOutputCurrentA){

    //Declare temporary message to receive data
    CANMessage msg;

    //If message received
    if(can.read(msg)){
        //Grab byte 1 and 2 of charger message MSB and LSB of output voltage respectively
        //Multiply by .1V/Byte to convert to V
        (*pOutputVoltageV) = ((msg.data[0] << 8) + msg.data[1])*0.1;

        //Grab byte 3 and 4 of charger message MSB and LSB of output current respectively
        //Multiply by .1A/Byte to convert to A
        (*pOutputCurrentA) = ((msg.data[2] << 8) + msg.data[3])*0.1;
        return 1;
    }
    //If message not received
    else{
        return 0;
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