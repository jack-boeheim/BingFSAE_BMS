#include "adbms_main.h"
#include "main.h"

// Set pin modes
DigitalIn miso(PIN_SPI_MISO);       // SPI MISO
DigitalIn driving(PIN_DRIVING);
DigitalIn charging(PIN_CHARGING);

DigitalOut master(MASTER_ENABLE);
DigitalOut chip_select(PIN_SPI_CS); // SPI chip select
DigitalOut mosi(PIN_SPI_MOSI);      // SPI MOSI
DigitalOut sclk(PIN_SPI_SCLK);      // SPI CLK

// Set SPI pins (MOSI, MISO, SCLK)
SPI spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCLK);

// Configure CAN
CAN can(PB_8, PB_9);


Serial pc(USBTX, USBRX);                    

Timer timer;

state_t FSM_state = INIT;

float cell_voltages[NUM_PACKS][NUM_CELLS];



/*-------------------------------------------------------------------------------------------------
 Main Loop
-------------------------------------------------------------------------------------------------*/
int main() {
    // Run the AD BMS application
    adbms_main();

    switch (FSM_state) {
        case (INIT):
            spi_init();

            // Send AWAKE message
            
            can.write( CANMessage(0x00, "AWAKE", 5, CANData, CANStandard) );

            // Check vehicle is active and charged
            if ( is_driving() && is_charging() ) {
                // Write config registers
                adBms6830_init_config(TOTAL_IC, &IC[0]);
                // Begin heartbeat
                
                // Check current sensor

                // Check battery cell voltage
                read_cell_voltages(1, &IC[0], );

                // Check the fans

                FSM_state = PRECHARGE;
            } else {
                FSM_state = FAULT;
            }
            
            break;

        case (PRECHARGE):
            break;
        
        case (DRIVE_MAIN):
            break;
        
        case (DRIVE_DEBUG):
            break;
        
        case (CHARGE):
            break;
        
        case (FAULT):
            break;
    }
    
    return 0;
}

