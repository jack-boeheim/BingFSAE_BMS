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

extern AnalogIn current_sensor;
extern AnalogIn temp_sensor;

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
 Check shutdown status
-----------------------------------------------------------------------------*/
bool is_shutdown_closed() {
    return shutdown_tap.read();
}

/*-----------------------------------------------------------------------------
 Initial check for current sensor is connected
-----------------------------------------------------------------------------*/
bool is_current_sensor_connected() {
    return false; // Placheholder for now
}

/*-----------------------------------------------------------------------------
 Set fault status to true
-----------------------------------------------------------------------------*/
void assert_fault_high(){
    fault = 1;
}

/*-----------------------------------------------------------------------------
 Set fault status to false
-----------------------------------------------------------------------------*/
void assert_fault_low(){
    fault = 0;
}

void voltage_can_message(cell_asic *IC) {
    // Define charger message parameters
    CANMessage can_v_msg;
    can_v_msg.id = CAN_CELL_V_ID;    // Set CAN ID 
    can_v_msg.format = CANStandard;  // Use standard CAN format (11-bit ID)
    can_v_msg.type = CANData;        // Message type (data frame) 
    can_v_msg.len = 8;               // Set length to 8 bytes

    // Add data for each cell into the message buffer
    for (int i = 0; i < NUM_MODULES;  ++i) {
        for(int j = 0; j < NUM_CELLS; ++j) {
            can_v_msg.data[0] = 12*i + j; // Cell ID
            can_v_msg.data[1] = ((IC[i].cell.c_codes[j] >> 8) & 0xFF); // 8 MSB of Instantaneous Cell Voltage 
            can_v_msg.data[2] = (IC[i].cell.c_codes[j] & 0xFF); // 8 LSB of Instantaneous Cell Voltage 
            can_v_msg.data[3] = 0; // 8 MSB of Internal Resistance  (placeholder 0 for now)
            can_v_msg.data[4] = 0; // 8 LSB of Internal Resistance  (placeholder 0 for now)
            can_v_msg.data[5] = 0; // 8 MSB of Open Circuit Voltage (placeholder 0 for now)
            can_v_msg.data[6] = 0; // 8 LSB of Open Circuit Voltage (placeholder 0 for now)
            can_v_msg.data[7] = (((CAN_CELL_V_ID + 8 + can_v_msg.data[0] + can_v_msg.data[1] + 
            can_v_msg.data[2] + can_v_msg.data[3] + can_v_msg.data[4] + 
            can_v_msg.data[5] + can_v_msg.data[6]) >> 8) & 0xFF); // Checksum (used same process as Orion)
        } 
        // Send the CAN message
        can.write(can_v_msg);      
    }
}

void charger_can_message(uint16_t maxChargeV, uint16_t maxChargeI, bool bChargeSafe) {
    // Define charger message parameters
    CANMessage can_charge_msg;
    can_charge_msg.id = CAN_CHARGER_MSG_ID;  // Set CAN ID 
    can_charge_msg.format = CANExtended;     // Use CAN Extended format (29-bit ID) for Charger
    can_charge_msg.type = CANData;           // Message type (data frame) 
    can_charge_msg.len = 8;                  // Set length to 8 bytes
   
    // CAN Data Frame per Elcon Charger Specifications 
    can_charge_msg.data[0] = ( (maxChargeV >> 8) & 0xFF ); 
    can_charge_msg.data[1] = (maxChargeV & 0xFF); 
    can_charge_msg.data[2] = ( (maxChargeI >> 8) & 0xFF ); 
    can_charge_msg.data[3] = (maxChargeI & 0xFF); 
    can_charge_msg.data[4] = bChargeSafe;

    // Send the CAN message
    can.write(can_charge_msg);
}


/*-----------------------------------------------------------------------------
 Get all BMS cell voltages stored in an array
-----------------------------------------------------------------------------*/
void get_cell_voltages(uint8_t tIC, cell_asic * IC, float ** data) {
    float voltage;
    int16_t temp;
    
    // Calculate voltage values for each cell
    for (uint8_t ic = 0; ic < tIC; ++ic) {
        for (uint8_t index = 0; index < NUM_CELLS; ++index) {
            temp = IC[ic].cell.c_codes[index];
            
            // Store voltage values in array
            data[ic][index] = getVoltage(temp);
        }
    }
}

/*-----------------------------------------------------------------------------
 Update all cell voltages
-----------------------------------------------------------------------------*/
void read_cell_voltages(uint8_t tIC, cell_asic * ic, float ** data) {
    adBmsWakeupIc(tIC);
    adBmsReadData(tIC, &ic[0], RDCVA, Cell, A);
    adBmsReadData(tIC, &ic[0], RDCVB, Cell, B);
    adBmsReadData(tIC, &ic[0], RDCVC, Cell, C);
    adBmsReadData(tIC, &ic[0], RDCVD, Cell, D);
    adBmsReadData(tIC, &ic[0], RDCVE, Cell, E);
    adBmsReadData(tIC, &ic[0], RDCVF, Cell, F);

    // Store updated cell voltages in array
    data = get_cell_voltages(tIC, &ic[0], data);
}

/*-----------------------------------------------------------------------------
 Check all battery cell voltages are valid
-----------------------------------------------------------------------------*/
bool check_cell_voltages(uint8_t tIC, cell_asic * IC, float ** data) {
    uint8_t count = 0;
    
    // Get the cell voltages
    read_cell_voltages(tIC, IC, data);

    // Check each cell voltage in each pack
    for(uint8_t cic = 0; cic < tIC; cic++) {
        for (uint8_t index = 0; index < NUM_CELLS; ++index) {
            // Increment counter if voltage is between UV and OV
            if ( data[cic][index] <= ic[cic].tx_cfgb.vov || data[cic][index] >= ic[cic].tx_cfgb.vuv ) {
                ++count;
            } 
        }
    }
    
    // Check all cells are a stable voltage
    return count == NUM_MODULES * NUM_CELLS;
}

/*-----------------------------------------------------------------------------
 Get the flag values 
-----------------------------------------------------------------------------*/
void read_all_flags(uint8_t tIC, cell_asic * IC, uint8_t * data) {
    adBmsReadData(TOTAL_IC, &IC[0], RDSTATA, Status, A);
    adBmsReadData(TOTAL_IC, &IC[0], RDSTATB, Status, B);
    adBmsReadData(TOTAL_IC, &IC[0], RDSTATC, Status, C);
    adBmsReadData(TOTAL_IC, &IC[0], RDSTATD, Status, D);
    adBmsReadData(TOTAL_IC, &IC[0], RDSTATE, Status, E);
    
    // data = get_;
}

/*-----------------------------------------------------------------------------
 Check all flag values
-----------------------------------------------------------------------------*/
bool check_all_flags() {
    ;
}

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
