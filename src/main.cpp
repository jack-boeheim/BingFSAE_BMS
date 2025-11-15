#include "adbms_main.h"
#include "adBms_Application.h"
#include "main.h"
#include <cstdint>
#include "functions.cpp"
#include "FlashIAPBlockDevice.h"


// initializing flash device
FlashIAPBlockDevice bd;
//function grah


// Set pin modes

DigitalIn driving(PIN_DRIVING);
DigitalIn charging(PIN_CHARGING);
DigitalIn shutdown_tap(PIN_SHUTDOWN);
DigitalOut fault(PIN_FAULT);


DigitalOut master(MASTER_ENABLE);
DigitalOut chip_select(PIN_SPI_CS); // SPI chip select
DigitalOut mosi(PIN_SPI_MOSI);      // SPI MOSI
DigitalIn miso(PIN_SPI_MISO);       // SPI MISO
DigitalOut sclk(PIN_SPI_SCLK);      // SPI CLK

// Set SPI pins (MOSI, MISO, SCLK)
SPI spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCLK);

// Configure CAN
CAN can(PB_8, PB_9);

//Configure Serial Communication for Debug 
//Serial pc(USBTX, USBRX);                    

Timer timer;
state_t FSM_state = INIT;
// assert_fault_high();
float cell_voltages[NUM_MODULES][NUM_CELLS];
cell_asic IC[NUM_MODULES];

/*-------------------------------------------------------------------------------------------------
 Main Loop
-------------------------------------------------------------------------------------------------*/
int main() {

    can.frequency(CAN_BAUD_RATE_CHARGE);
    spi_init();
    adBms6830_init_config(TOTAL_IC, &IC[0]);

    adBms6830_write_config(TOTAL_IC, &IC[0]); 
    adBms6830_start_adc_cell_voltage_measurment(TOTAL_IC);
    adBms6830_start_adc_s_voltage_measurment(TOTAL_IC);
    adBms6830_start_aux_voltage_measurment(TOTAL_IC, &IC[0]);
    flash_test();
    
    while(1)
    {
        
    }


    // switch (FSM_state) {
    //     case (INIT):
    //         fault = 1;
    //         Delay_ms(1);
    //         spi_init(); 

    //         // Send AWAKE message
    //       //   can.write(CANMessage(0x00, "AWAKE", 5, CANData, CANStandard) );

    //         // Check if Accumulator is in drive mode
    //         if (is_driving() && !is_charging()) {
    //             // Write config registers
    //             adBms6830_init_config(TOTAL_IC, &IC[0]);

    //             // Check current sensor (use same method as ECU code to cook value)
    //             // Check battery cell voltage (run reading and check OV/UV flag & others)
    //             // read_cell_voltages(1, &IC[0], );
    //             // Check the fans (Joe to add fan check function)
    //              // Will need to check all Flgas before doing this (maybe create a function to do so)
    //             // assert_fault_low();
    //             FSM_state = PRECHARGE;
    //         } 
    //         else if(is_charging() && !is_driving()) {
    //             FSM_state = CHARGE;
    //         }

    //         else {
    //             FSM_state = FAULT;
    //         }
            
    //         break;

    //     case (PRECHARGE):
    //         fault = 0;
    //             while(!is_shutdown_closed()){
    //                 //Wait until SDC is closed
    //             }
    //         Delay_ms(500); //Wait for Precharge (500ms after SDC closed) (temporary should accept input from Tractive Precharge control board)    
    //         FSM_state = DRIVE_MAIN;
    //         break;
        
    //     case (DRIVE_MAIN): 
          
    //       can.frequency(CAN_BAUD_RATE_DRIVE);
    //       adBms6830_write_config(TOTAL_IC, &IC[0]); 
    //       adBms6830_start_adc_cell_voltage_measurment(TOTAL_IC);
    //       adBms6830_start_adc_s_voltage_measurment(TOTAL_IC);
    //       adBms6830_start_aux_voltage_measurment(TOTAL_IC, &IC[0]);

    //         while(is_shutdown_closed()){
    //            measurement_loop();
    //            //voltage_can_message(&IC[0]);
    //            Delay_ms(10);
    //         }
            
    //         FSM_state = FAULT;
            
    //         break;
        
    //     case (DRIVE_DEBUG):
    //      //Need to figure out a good way to switch into this state (code macro, or input pin)
    //         can.frequency(CAN_BAUD_RATE_DRIVE);
    //         break;
        
    //     case (CHARGE):
            
    //          can.frequency(CAN_BAUD_RATE_CHARGE);
    //          measurement_loop();
    //          //voltage_can_message(&IC[0]);
    //         break;
        
    //     case (FAULT):
    //        // bms_fault = 1;  
    //       //  assert_fault_high();
    //       // Include CAN message showing Fault Code 
    //         break;
    // }
    
    // return 0;
}

