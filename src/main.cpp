#include "adbms_main.h"
#include "main.h"
    
Timer timer;

state_t FSM_State = INIT;

int main() {
    switch (FSM_State) {
        case (INIT):
            spi_init();

            // Run the MBED BMS application
            adbms_main();
            
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
