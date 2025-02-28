
#include "adbms_main.h"
#include "adBms_Application.h"
#include "main.h"


DigitalIn driving(PIN_DRIVING);
DigitalIn charging(PIN_CHARGING);
DigitalIn shutdown_tap(PIN_SHUTDOWN);
DigitalOut fault(PIN_FAULT);
DigitalOut master(MASTER_ENABLE);
DigitalOut chip_select(PIN_SPI_CS); // SPI chip select
DigitalOut mosi(PIN_SPI_MOSI);      // SPI MOSI
DigitalIn miso(PIN_SPI_MISO);       // SPI MISO
DigitalOut sclk(PIN_SPI_SCLK);      // SPI CLK
PwmOut fan1(PIN_FAN_1);  
SPI spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCLK);
CAN can(PB_8, PB_9);
Timer timer;

//Variable Declarations 
state_t FSM_state = INIT;
CANMessage can_msg_rx, can_msg_tx;
uint16_t CellErrorBuf[NUM_MODULES];
cell_asic IC[NUM_MODULES];

/*-------------------------------------------------------------------------------------------------
 Main Loop
-------------------------------------------------------------------------------------------------*/
int main() {

    switch (FSM_state) {
        case (INIT):
            printf("init");

            set_fault_high();
            Delay_ms(1);
            spi_init(); 

            // Send AWAKE message
            //   can.write(CANMessage(0x00, "AWAKE", 5, CANData, CANStandard) );

            // Check if Accumulator is in drive mode
            if (is_driving() && !is_charging()) {
                // Write config registers
                adBms6830_init_config(TOTAL_IC, &IC[0]);

                // Check current sensor (use same method as ECU code to cook value)
                // Check battery cell voltage (run reading and check OV/UV flag & others)
                // read_cell_voltages(1, &IC[0], );
                // Check the fans (Joe to add fan check function)
                 // Will need to check all Flgas before doing this (maybe create a function to do so)
                // assert_fault_low();
                FSM_state = PRECHARGE;
            } 
            else if(is_charging() && !is_driving()) {
                FSM_state = CHARGE;
            }

            else {
                FSM_state = FAULT;
            }
            
            break;

        case (PRECHARGE):
            printf("precharge");

            fault = 0;
                while(!is_shutdown_closed())
                    continue;
            Delay_ms(500); //Wait for Precharge (500ms after SDC closed) (temporary should accept input from Tractive Precharge control board)    
            FSM_state = DRIVE_MAIN;
            break;
        
        case (DRIVE_MAIN): 

            printf("dmain");
          
            can.frequency(CAN_BAUD_RATE_DRIVE);
            adBms6830_write_config(TOTAL_IC, &IC[0]); 
            adBms6830_start_adc_cell_voltage_measurment(TOTAL_IC);
            adBms6830_start_adc_s_voltage_measurment(TOTAL_IC);
            adBms6830_start_aux_voltage_measurment(TOTAL_IC, &IC[0]);

            while(is_shutdown_closed()){
               measurement_loop();
               voltage_can_message(&IC[0],&can_msg_tx);
               Delay_ms(10);
            }
            
            FSM_state = FAULT;
            
            break;
        
        case (DRIVE_DEBUG):
            printf("debug");

         //Need to figure out a good way to switch into this state (code macro, or input pin)
            can.frequency(CAN_BAUD_RATE_DRIVE);
            break;
        
        case (CHARGE):
            printf("charge");

            can.frequency(CAN_BAUD_RATE_CHARGE);
            measurement_loop();
            voltage_can_message(&IC[0],&can_msg_tx);
            break;
        
        case (FAULT):
            printf("fault");

            set_fault_high();
            // Include CAN message showing Fault Code 
            break;
    }
    
    return 0;
}


