/**
********************************************************************************
*
* @file:    application.c
*
* @brief:   This file contains the application test cases.
*
* @details:
*
*******************************************************************************
Copyright(c) 2020 Analog Devices, Inc. All Rights Reserved. This software is
proprietary & confidential to Analog Devices, Inc. and its licensors. By using
this software you agree to the terms of the associated Analog Devices License
Agreement.
*******************************************************************************
*/
/*! \addtogroup Main
*  @{
*/

/*! \addtogroup Application
*  @{
*/
/*============= I N C L U D E S =============*/
/*============== D E F I N E S ===============*/
/*============= E X T E R N A L S ============*/
/*============= E N U M E R A T O R S ============*/

#include "application.h"
#ifdef MBED
//extern Serial pc;
#endif /* MBED */
/**
*******************************************************************************
* @brief Setup Variables
* The following variables can be modified to configure the software.
*******************************************************************************
*/

#define TOTAL_IC 1
cell_asic IC[TOTAL_IC];
int loop_measurment_count = 10;      /* Loop measurment count (default count)*/
int loop_measurment_time = 1;        /* milliseconds(mS)*/
int loop_count = 0;

void app_main()
{
  printMenu();
  adi2950_init_config(TOTAL_IC, &IC[0]);
  while(1)
  {
    int user_command;
    readUserInupt(&user_command);
    run_command(user_command); /*!< Run test case */
  }
}

void run_command(int cmd)
{
  switch(cmd)
  {
  case 1:
    // Write and Read Configuration
    adi2950_write_read_config(TOTAL_IC, &IC[0]);
    break;

  case 2:
    // Read Configuration
    adi2950_read_config(TOTAL_IC, &IC[0]);
    break;

  case 3:
    // Start ADI1 Conversion(Single Shot)
    adi2950_start_adi1_single_measurment(TOTAL_IC, &IC[0]);
    break;

  case 4:
    // Start ADI2 Conversion(Single Shot)
    adi2950_start_adi2_single_measurment(TOTAL_IC, &IC[0]);
    break;

  case 5:
    // Start ADI1 Conversion(Continuous)
    adi2950_start_adi1_continuous_measurment(TOTAL_IC, &IC[0]);
    break;

  case 6:
    // Start ADI2 Conversion(Continuous)
    adi2950_start_adi2_continuous_measurment(TOTAL_IC, &IC[0]);
    break;

  case 7:
    // Start ADI1 Redundant Conversion(Single Shot)
    adi2950_start_adi1_redundant_single_measurment(TOTAL_IC, &IC[0]);
    break;

  case 8:
    // Start ADI1 Redundant Conversion(Continuous)
    adi2950_start_adi1_redundant_continuous_measurment(TOTAL_IC, &IC[0]);
    break;

  case 9:
    // Read CR, VBAT & IVBAT Registers(Single Shot)
    adi2950_read_cr_vbat_ivbat_registers(TOTAL_IC, &IC[0]);
    break;

  case 10:
    // Read CR, VBAT & IVBAT Registers(Continuous)
    loop_count = 0;
    cmd_description command;
    printMsg("Please enter loop measurment count:");
    readUserInupt(&loop_measurment_count);
    // Wakeup
    adBmsWakeupIc(TOTAL_IC);    
    // ADI1 command with RD off and continuous without diagnostic
    // adBms2950_Adi1(TOTAL_IC, &IC[0], RD_OFF, OPT8_C, &command);
    // Delay_ms(ADI1_delay_ms); 
    while(loop_count < loop_measurment_count)
    {
      printResultCount(loop_count);
      adi2950_read_cr_vbat_ivbat_registers(TOTAL_IC, &IC[0]);
      Delay_ms(loop_measurment_time);
      loop_count++;
    }
    break;

  case 11:
    // Read Overcurrent ADC Register(Single Shot)
    adi2950_read_ocr_register(TOTAL_IC, &IC[0]);
    break;

  case 12:
    // Read Overcurrent ADC Register(Continuous)
    loop_count = 0;
    printMsg("Please enter loop measurment count:");
    readUserInupt(&loop_measurment_count);
    // Wakeup
    adBmsWakeupIc(TOTAL_IC);    
    while(loop_count < loop_measurment_count)
    {
      printResultCount(loop_count);
      adi2950_read_ocr_register(TOTAL_IC, &IC[0]);
      Delay_ms(loop_measurment_time);
      loop_count++;
    }
    break;

  case 13:
    // Read Accumulated CR, VBAT & IVBAT Registers(Single Shot)
    adi2950_read_acccr_accvbat_accivbat_registers(TOTAL_IC, &IC[0]);
    break;

  case 14:
    // Read Accumulated CR, VBAT & IVBAT Registers(Continuous)
    loop_count = 0;
    printMsg("Please enter loop measurment count:");
    readUserInupt(&loop_measurment_count);
    // Wakeup
    adBmsWakeupIc(TOTAL_IC);    
    while(loop_count < loop_measurment_count)
    {
      printResultCount(loop_count);
      adi2950_read_acccr_accvbat_accivbat_registers(TOTAL_IC, &IC[0]);
      Delay_ms(loop_measurment_time);
      loop_count++;
    }
    break;

  case 15:
    // Read All CR and VBAT Voltage Registers(Single Shot)
    adi2950_all_current_battery_voltage_registers(TOTAL_IC, &IC[0]);
    break;

  case 16:
    // Read All CR and VBAT Voltage Registers(Continuous)
    printMsg("Please enter loop measurment count:");
    readUserInupt(&loop_measurment_count);
    // Wakeup
    adBmsWakeupIc(TOTAL_IC);    
    loop_count = 0;
    while(loop_count < loop_measurment_count)
    {
      printResultCount(loop_count);
      adi2950_all_current_battery_voltage_registers(TOTAL_IC, &IC[0]);
      Delay_ms(loop_measurment_time);
      loop_count++;
    }
    break;

  case 17:
    // Start ADV All Channel Conversion
    adi2950_start_adv_measurment(TOTAL_IC, &IC[0]);
    break;

  case 18:
    // Read All VR Registers(RDV commands)
    adi2950_read_vr_registers(TOTAL_IC, &IC[0]);
    break;

  case 19:
    // Read All RVR Registers(RDRVA & RDRVB commands)
    adi2950_read_rvr_registers(TOTAL_IC, &IC[0]);
    break;

  case 20:
    // Read All VR & RVR Registers(RDV, RDRVA & RDRVB commands)
    adi2950_read_vr_rvr_registers(TOTAL_IC, &IC[0]);
    break;

   case 21:
    // Read Voltage Registers(RDAUXC & RDAUXD commands)
    adi2950_read_vrx_registers(TOTAL_IC, &IC[0]);
    break;

   case 22:
    // Wakeup
    adBmsWakeupIc(TOTAL_IC);    
    spiSendCmd(TOTAL_IC, &IC[0], CLRVX); // CLRAUX to clear status register A
    // Start ADAUX All Channel Conversion
    // adi2950_start_adaux_measurment(TOTAL_IC);
    spiSendCmd(TOTAL_IC, &IC[0], sADX);
#ifdef POLLING
    // PLX to poll on the conversion
    IC[0].pladc_count = adBmsPollAdc(TOTAL_IC, &IC[0], PLX);
#else
    Delay_ms(Polling_Delay_ms);
#endif
    // Read Aux voltages and VREF2
    adBmsReadData(TOTAL_IC, &IC[0], RDALLX, Rdallx, ALL_GRP);
    // printAux(TOTAL_IC, &IC[0], Aux, ALL_GRP);
    printRdallx(TOTAL_IC, &IC[0]);
    spiSendCmd(TOTAL_IC, &IC[0], CLRVX); // CLRAUX to clear status register A
    break;

   case 23:
    // Read ADAUX Measurement
    adi2950_read_adaux_measurment(TOTAL_IC, &IC[0]);
    break;

   case 24:
    // Read Status and Flag Register
    adi2950_read_status_register(TOTAL_IC, &IC[0]);
    break;

   case 25:
    // Read All Status Registers
    adi2950_read_all_status_registers(TOTAL_IC, &IC[0]);
    break;

   case 26:
    // Read Device SID
    adi2950_read_device_sid(TOTAL_IC, &IC[0]);
    break;

   case 27:
    // Soft Reset
    adi2950_soft_reset(TOTAL_IC, &IC[0]);
    break;

   case 28:
    // Reset cmd counter
    adi2950_reset_cmd_count(TOTAL_IC, &IC[0]);
    break;

   case 29:
    // SNAP(Stop Reg. updates)
    adi2950_snap(TOTAL_IC, &IC[0]);
    break;

   case 30:
    // UNSNAP(Resume Reg. updates)
    adi2950_unsnap(TOTAL_IC, &IC[0]);
    break;

   case 31:
    // Set Reset GPO Pins
    adi2950_set_reset_gpo_pins(TOTAL_IC, &IC[0]);
    break;

   case 32:
    // GPIO SPI Write to Slave
    adi2950_gpio_spi_write_to_slave(TOTAL_IC, &IC[0]);
    break;

   case 33:
    // GPIO SPI Read from Slave
    adi2950_gpio_spi_read_from_slave(TOTAL_IC, &IC[0]);
    break;

   case 34:
    // GPIO I2C Write to Slave
    adi2950_gpio_i2c_write_to_slave(TOTAL_IC, &IC[0]);
    break;

   case 35:
    // GPIO I2C Read from Slave
    adi2950_gpio_i2c_read_from_slave(TOTAL_IC, &IC[0]);
    break;

  case 0:
    printMenu();
    break;

  default:
    printMsg("Incorrect Option");
    break;
  }
}
/** @}*/
/** @}*/