#include "main.h"
#include "adBms_Application.h"
#include "serialPrintResult.h"
#include "mcuWrapper.h"
#include <cstdint>
#include <unordered_map>

extern SPI spi; 
extern CAN can;
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

/*-----------------------------------------------------------------------------
 Send CAN Functions
-----------------------------------------------------------------------------*/

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


/*-----------------------------------------------------------------------------
 Read CAN Functions
-----------------------------------------------------------------------------*/
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
 OV/UV Detection
-----------------------------------------------------------------------------*/
bool check_OV_UV_flags(cell_asic *IC, uint16_t *pCellErrorBuf){
/*
    Flags Fetched from measurement_loop function and stored in IC[].statcd.
    This will work for now but can be made more efficient by check the flag directly
    as it is read in so the data is not looped over twice 
    (will require editiing of adBms6830ParseStatusD in adBmsParseCreate.cpp)

    Must call:
        measurement_loop(); THEN
        adBms6830_read_status_registers(TOTAL_IC, IC); THEN
        check_OV_UV_flags(IC, pCellErrorBuf);

*/
   uint8_t errorCnt = 0;

    for(int i = 0; i < NUM_MODULES; ++i){
        for(int j = 0; j < NUM_CELLS_PER_MODULE; ++j){
            if(IC[i].statd.c_ov[j]){
                pCellErrorBuf[i] |= (1<<j); //Ecode Problem Cell as One-Hot w/ Cell Location in module
                errorCnt++;
            }
            else if(IC[i].statd.c_uv[j]){
                 pCellErrorBuf[i] |= (1<<j);
                 errorCnt++;
            }
        }
    }

    if(errorCnt > 0)
    {
        for(int module_ix = 0; module_ix < NUM_MODULES; module_ix++)
        {
            printf("OV/UV Fault Detected in Module %d Cells ", module_ix);
            for(int cell_ix = 0; cell_ix < NUM_CELLS_PER_MODULE; cell_ix++)
            {
                if((pCellErrorBuf[module_ix]) & (1<<cell_ix))
                {
                    printf("%d ", cell_ix);
                }
            }
            printf("\n\n");
        }
    }

    return errorCnt > 0;
}

/*-----------------------------------------------------------------------------
 Get all BMS cell voltages stored in an array
-----------------------------------------------------------------------------*/
void get_cell_voltages(uint8_t tIC, cell_asic * IC, float ** data) {
    float voltage;
    int16_t temp;
    for (uint8_t module_ix = 0; module_ix < tIC; ++module_ix) {
        for (uint8_t cell_ix = 0; cell_ix < NUM_CELLS_PER_MODULE; ++cell_ix) {
            temp = IC[module_ix].cell.c_codes[cell_ix];
            voltage = getVoltage(temp);
            data[module_ix][cell_ix] = voltage;
        }
    }
}

#define ENEPAQ_LUT_SIZE 33
#define ENEPAQ_TEMP_START -40.0f
#define ENEPAQ_TEMP_STEP  5.0f

static const float enepaq_voltage_lut[ENEPAQ_LUT_SIZE] = {
    2.44f, 2.42f, 2.40f, 2.38f, 2.35f, 2.32f, 2.27f, 2.23f, 2.17f, 2.11f, 
    2.05f, 1.99f, 1.92f, 1.86f, 1.80f, 1.74f, 1.68f, 1.63f, 1.59f, 1.55f, 
    1.51f, 1.48f, 1.45f, 1.43f, 1.40f, 1.38f, 1.37f, 1.35f, 1.34f, 1.33f, 
    1.32f, 1.31f, 1.30f
};

float get_temp_from_enepaq_therm_voltage(cell_asic IC, uint8_t gpio_pin)
{
    float voltage = get_aux_voltage(IC, gpio_pin);
    
    if(voltage < enepaq_voltage_lut[ENEPAQ_LUT_SIZE - 1]) return 130.0f; // Return max temp + 10
    else if (voltage > enepaq_voltage_lut[0]) return -50.0f; // Return min temp - 10

    uint8_t lut_ix = 0;

    while(enepaq_voltage_lut[lut_ix] > voltage) lut_ix++; // Find first element in LUT that is less than voltage

    if(lut_ix != 0)
    {
        float diff_from_prev = enepaq_voltage_lut[lut_ix - 1] - voltage;
        float diff_from_cur = voltage - enepaq_voltage_lut[lut_ix];
        if( diff_from_prev < diff_from_cur ) lut_ix--;
    }

    return ENEPAQ_TEMP_START + (ENEPAQ_TEMP_STEP * lut_ix);
}

float get_aux_voltage(cell_asic IC, uint8_t gpio_pin)
{
    return getVoltage(IC.aux.a_codes[gpio_pin]);
}

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