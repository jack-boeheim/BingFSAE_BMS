#include "adbms_main.h"
#include "adBms_Application.h"
#include "main.h"

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

AnalogIn current_sensor(PIN_CURRENT_SENSOR);
// AnalogIn temp_sensor(PIN_TEMP_SENSOR);

// Set SPI pins (MOSI, MISO, SCLK)
SPI spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCLK);

// Configure CAN
CAN can(PB_8, PB_9);

// Configure Serial Communication for Debug 
Serial pc(USBTX, USBRX);                    

Timer timer;
state_t FSM_State = INIT;
// assert_fault_high();

float cell_voltages[NUM_MODULES][NUM_CELLS];
cell_asic IC[NUM_MODULES];


/*-------------------------------------------------------------------------------------------------
 Main Loop
-------------------------------------------------------------------------------------------------*/
int main() { while (1) {
    switch (FSM_State) {
        /*-----------------------------------------------------------------------------
         INIT state
        -----------------------------------------------------------------------------*/
        case (INIT):
            assert_fault_low();
            Delay_ms(1);
            spi_init(); 

            // Send AWAKE message
            // can.write( CANMessage(0x00, "AWAKE", 5, CANData, CANStandard) );

            // Check if Accumulator is in drive mode
            if ( is_driving() && !is_charging() ) {
                // Write config registers
                adBms6830_init_config(TOTAL_IC, &IC[0]);

                // Check current sensor (use same method as ECU code to cook value)
                if ( !is_current_sensor_connected() ) {
                    FSM_State = FAULT;
                    break;
                }

                // Check battery cell voltage (run reading and check OV/UV flag & others)
                if ( !check_cell_voltages(NUM_MODULES, IC, cell_voltages) ) {
                    FSM_State = FAULT;
                    break;
                }
                
                // Check the fans (Joe to add fan check function)
                

                // Will need to check all flags before doing this (maybe create a function to do so)
                if ( !check_all_flags() ) {
                    FSM_State = FAULT;
                    break;
                }

                assert_fault_low();

                FSM_State = PRECHARGE;
            } else if ( is_charging() && !is_driving() ) {
                FSM_State = CHARGE;
            } else { 
                FSM_State = FAULT;
            }

            break;
        /*-----------------------------------------------------------------------------
         PRECHARGE state
        -----------------------------------------------------------------------------*/
        case (PRECHARGE):
            // Check current sensor should be at 0A else fault
            if ( !current_sensor.read_16u() ) {
                FSM_State = FAULT;
                break;
            }

            assert_fault_low();

            // Wait until SDC is closed to start measuring data
            if ( is_shutdown_closed() ) {
                // Wait for Precharge (500ms after SDC closed) (temporary should accept input from Tractive Precharge control board)    
                Delay_ms(PRECHARGE_TIME);

                #ifdef DEBUG
                    FSM_State = DRIVE_DEBUG;
                #else
                    FSM_State = DRIVE_MAIN;
                #endif
            }
        
            break;
        
        /*-----------------------------------------------------------------------------
         DRIVING state
        -----------------------------------------------------------------------------*/
        case (DRIVE_MAIN):
            // All 8 cells contain 10 temperature sensors need to check all

            // Record measurements when shutdown is closed and temperature sensor reading is good
            if ( !is_shutdown_closed() ) {
                FSM_State = FAULT;
                break;
            }

            // Start ADC conversion
            can.frequency(CAN_BAUD_RATE_DRIVE);
            adBms6830_write_config(TOTAL_IC, &IC[0]); 
            adBms6830_start_adc_cell_voltage_measurment(TOTAL_IC);
            adBms6830_start_adc_s_voltage_measurment(TOTAL_IC);
            adBms6830_start_aux_voltage_measurment(TOTAL_IC, &IC[0]);

            // Read cell voltages from registers
            measurement_loop();

            // Send voltages CAN message
            voltage_can_message( &IC[0] );

            Delay_ms(10);

            // Update fan speed

            // Send CAN data package, SOC, all voltages, highest temp
            // can.write();
            // can.write();
            // can.write();

            // Fault when temperature readings fail / shutdown TAP opens
            FSM_State = FAULT;
            break;

        /*-----------------------------------------------------------------------------
         DRIVING state (debugging)
        -----------------------------------------------------------------------------*/
        case (DRIVE_DEBUG):
            // Need to figure out a good way to switch into this state (code macro, or input pin)

            // Set CAN baud rate
            can.frequency(CAN_BAUD_RATE_DRIVE);

            // Send cell / temperature data to GUI, status register
            measurement_loop();

            break;
        
        /*-----------------------------------------------------------------------------
         CHARGE state
        -----------------------------------------------------------------------------*/
        case (CHARGE):
            // Set CAN baud rate
            can.frequency(CAN_BAUD_RATE_CHARGE);

            // Send terminal message to set current limit


            // Send terminal message current cell/temp readings, update until user input to start charging


            // Send CAN message to Elcon (power request)
            charger_can_message(0, 0, 0); // Placeholders for now

            // Read cell voltages from registers
            measurement_loop();
            voltage_can_message( &IC[0] );
            
            FSM_state = DRIVE_MAIN;
            break;
        
        /*-----------------------------------------------------------------------------
         FAULT state
        -----------------------------------------------------------------------------*/
        case (FAULT):
            assert_fault_high();

            // Include CAN message showing Fault Code 
            can.write( CANMessage(0x00, 0, 0, CANData, CANStandard) ); // Placeholders for now

            break;
    }
}
    // Continuously update cell voltages
    read_cell_voltages(NUM_MODULES, IC, cell_voltages);

    return 0;
}
