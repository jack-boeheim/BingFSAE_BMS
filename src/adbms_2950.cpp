/**
********************************************************************************
*
* @file:    adbms_2950.c
*
* @brief:   This file contains the test cases implementation.
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

/*! \addtogroup Test_Cases
*  @{
*/
#include "adbms2950.h"
#include "application.h"
#ifdef MBED
//extern Serial pc;
#endif /* MBED */
/*******************************************************************************
* @brief Setup Variables
* The following variables can be modified to configure the software.
*******************************************************************************/
/*!< ********************************GLOBAL VARIABLES****************************/
/*!< ADC Command Configurations */
VCH   VOLTAGE_MEASUREMENT     = SM_V1;
RD    REDUNDANT_MEASUREMENT   = RD_OFF;
//ACH   AUX_CH_TO_CONVERT       = ALL;
//CONT  CONTINUOUS_MEASUREMENT  = SINGLE;
OW    OW_WIRE_DETECTION       = OW_OFF;
ERR   INJECT_ERR_SPI_READ     = WITHOUT_ERR;
//uint32_t pladc_count;
const uint32_t ITER_LOOP_MEASUREMENT_COUNT = 1;      /* Loop measurment count */
const uint16_t MEASUREMENT_LOOP_TIME  = 10;     /* milliseconds(mS)*/
uint32_t loop_counter = 0;

/*!< ****************************************************************************/

/**
*******************************************************************************
* @brief Set configuration register A. Refer to the data sheet
*        Set configuration register B. Refer to the data sheet
*******************************************************************************
*/
void adi2950_init_config(uint8_t tIC, cell_asic *ic)
{
  for(int cic=0; cic<tIC; cic++)
  {
    //CFGA
    ic[cic].tx_cfga.gpo1c = PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo2c = PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo3c = PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo4c = PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo5c = PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo6c = PULLED_UP_TRISTATED;
    
    ic[cic].tx_cfga.gpo1od = OPEN_DRAIN;
    ic[cic].tx_cfga.gpo2od = OPEN_DRAIN;
    ic[cic].tx_cfga.gpo3od = OPEN_DRAIN;
    ic[cic].tx_cfga.gpo4od = OPEN_DRAIN;
    ic[cic].tx_cfga.gpo5od = OPEN_DRAIN;
    ic[cic].tx_cfga.gpo6od = OPEN_DRAIN;   
    
    ic[cic].tx_cfga.vs1  = VSM_SGND;
    ic[cic].tx_cfga.vs2  = VSM_SGND;
    ic[cic].tx_cfga.vs3  = VSMV_SGND;      
    ic[cic].tx_cfga.vs4  = VSMV_SGND;
    ic[cic].tx_cfga.vs5  = VSMV_SGND;
    ic[cic].tx_cfga.vs6  = VSMV_SGND;
    ic[cic].tx_cfga.vs7  = VSMV_SGND;
    ic[cic].tx_cfga.vs8  = VSMV_SGND;      
    ic[cic].tx_cfga.vs9  = VSMV_SGND;
    ic[cic].tx_cfga.vs10 = VSMV_SGND; 
    
    ic[cic].tx_cfga.injosc = INJOSC0_NORMAL;
    ic[cic].tx_cfga.injmon = INJMON0_NORMAL;
    ic[cic].tx_cfga.injts  = NO_THSD;
    ic[cic].tx_cfga.injecc = NO_ECC;
    ic[cic].tx_cfga.injtm  = NO_TMODE; 
    
    ic[cic].tx_cfga.soak    = SOAK_DISABLE;
    ic[cic].tx_cfga.ocen    = OC_DISABLE;
    ic[cic].tx_cfga.gpio1fe = FAULT_STATUS_DISABLE; 
    ic[cic].tx_cfga.spi3w   = FOUR_WIRE; 
    
    ic[cic].tx_cfga.acci    = ACCI_8;
    ic[cic].tx_cfga.commbk  = COMMBK_OFF; 
    ic[cic].tx_cfga.vb1mux  = SINGLE_ENDED_SGND;
    ic[cic].tx_cfga.vb2mux  = SINGLE_ENDED_SGND;  
   
    //CFGB
    ic[cic].tx_cfgb.gpio1c = PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio2c = PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio3c = PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio4c = PULL_DOWN_OFF;
    
    ic[cic].tx_cfgb.oc1th = 0x0; 
    ic[cic].tx_cfgb.oc2th = 0x0; 
    ic[cic].tx_cfgb.oc3th = 0x0; 
    
    ic[cic].tx_cfgb.oc1ten = NORMAL_INPUT;
    ic[cic].tx_cfgb.oc2ten = NORMAL_INPUT;
    ic[cic].tx_cfgb.oc3ten = NORMAL_INPUT;
    
    ic[cic].tx_cfgb.ocdgt  = OCDGT0_1oo1; 
    ic[cic].tx_cfgb.ocdp   = OCDP0_NORMAL; 
    ic[cic].tx_cfgb.reften = NORMAL_INPUT; 
    ic[cic].tx_cfgb.octsel = OCTSEL0_OCxADC_P140_REFADC_M20;
    
    ic[cic].tx_cfgb.ocod   = PUSH_PULL; 
    ic[cic].tx_cfgb.oc1gc  = GAIN_1;
    ic[cic].tx_cfgb.oc2gc  = GAIN_1;
    ic[cic].tx_cfgb.oc3gc  = GAIN_1;
    ic[cic].tx_cfgb.ocmode = OCMODE0_DISABLED;
    ic[cic].tx_cfgb.ocax   = OCABX_ACTIVE_HIGH;
    ic[cic].tx_cfgb.ocbx   = OCABX_ACTIVE_HIGH;
    
    ic[cic].tx_cfgb.diagsel   = DIAGSEL0_IAB_VBAT;
    ic[cic].tx_cfgb.gpio2eoc  = EOC_DISABLED; 
  }
}

/**
*******************************************************************************
* @brief Write and Read Configuration Register A/B
*******************************************************************************
*/
void adi2950_write_read_config(uint8_t tIC, cell_asic *ic)
{
  //---------------------Header---------------------------------------------//
  // Wake up, SRST, RDSID, RDSTAT, RDCFGA, RDFLAG, CLRFLAG
  adBms2950_Functional_header(tIC, &ic[0]);

  //---------------------------- CFGA ------------------------------------------
  //CFGAR0
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.vs1       = (VS)VSM_V4;
    ic[cic].tx_cfga.vs2       = (VS)VSM_V4;
    ic[cic].tx_cfga.vs3       = (VSB)VSMV_VREF1P25;      
    ic[cic].tx_cfga.vs4       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.vs5       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.ocen      = (OCEN)OC_ENABLE;   
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //CFGAR1
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */      
    ic[cic].tx_cfga.injosc    = (INJOSC)INJOSC1_OSC_FAST;
    ic[cic].tx_cfga.injmon    = (INJMON)INJMON3_OV_DE;
    ic[cic].tx_cfga.injts     = (INJTS)FORCE_THSD;
    ic[cic].tx_cfga.injecc    = (INJECC)FORCE_ECC;
    ic[cic].tx_cfga.injtm     = (INJTM)FORCE_TMODE;      
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //CFGAR2
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.vs6       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.vs7       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.vs8       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.vs9       = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.vs10      = (VSB)VSMV_VREF1P25;
    ic[cic].tx_cfga.soak      = (SOAK)SOAK_150ms;      
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //CFGAR3
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */      
    ic[cic].tx_cfga.gpo1c     = (GPOxC)PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo2c     = (GPOxC)PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo3c     = (GPOxC)PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo4c     = (GPOxC)PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo5c     = (GPOxC)PULLED_UP_TRISTATED;
    ic[cic].tx_cfga.gpo6c     = (GPOxC)PULLED_UP_TRISTATED;      
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //CFGAR4
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */      
    ic[cic].tx_cfga.gpo1od    = (GPOxOD_OPOD)OPEN_DRAIN;
    ic[cic].tx_cfga.gpo2od    = (GPOxOD_OPOD)OPEN_DRAIN;
    ic[cic].tx_cfga.gpo3od    = (GPOxOD_OPOD)OPEN_DRAIN;
    ic[cic].tx_cfga.gpo4od    = (GPOxOD_OPOD)OPEN_DRAIN;
    ic[cic].tx_cfga.gpo5od    = (GPOxOD_OPOD)OPEN_DRAIN;
    ic[cic].tx_cfga.gpo6od    = (GPOxOD_OPOD)OPEN_DRAIN; 
    ic[cic].tx_cfga.gpio1fe   = (GPIO1FE)FAULT_STATUS_ENABLE; 
    ic[cic].tx_cfga.spi3w     = (SPI3W)THREE_WIRE; 
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //CFGAR5
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config A */
    ic[cic].tx_cfga.acci      = (ACCI)ACCI_32;
    ic[cic].tx_cfga.commbk    = (COMMBK)COMMBK_ON; //uncomment this if running in daisy chain as this will affect communication in daisy chain
    //      ic[cic].tx_cfga.snapst    = (SNAPST)SNAP_ON; //snapst is read bits, Just to check if this bits writes to register (commented in Rev B)
    ic[cic].tx_cfga.vb1mux    = (VBxMUX)DIFFERENTIAL;
    ic[cic].tx_cfga.vb2mux    = (VBxMUX)DIFFERENTIAL;   
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);   

  //---------------------------- CFGB ------------------------------------------
  //CFGBR0
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.oc1th = 0x7F; //setting oc1th Max
    ic[cic].tx_cfgb.oc1ten = (OCxTEN_REFTEN)REFERENCE_INPUT;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //CFGBR1
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.oc2th = 0x7F; //setting oc1th Max
    ic[cic].tx_cfgb.oc2ten = (OCxTEN_REFTEN)REFERENCE_INPUT;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //CFGBR2
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.oc3th = 0x7F; //setting oc1th Max
    ic[cic].tx_cfgb.oc3ten = (OCxTEN_REFTEN)REFERENCE_INPUT;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //CFGBR3
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.ocdgt     = (OCDGT)OCDGT3_7oo8; 
    ic[cic].tx_cfgb.ocdp      = (OCDP)OCDP1_FAST; 
    ic[cic].tx_cfgb.reften    = (OCxTEN_REFTEN)REFERENCE_INPUT; 
    ic[cic].tx_cfgb.octsel    = (OCTSEL)OCTSEL3_OCxADC_M293_REFADC_P20;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //CFGBR4
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /* Init config B */
    ic[cic].tx_cfgb.ocod      = (GPOxOD_OPOD)OPEN_DRAIN; 
    ic[cic].tx_cfgb.oc1gc     = (OCxGC)GAIN_2;
    ic[cic].tx_cfgb.oc2gc     = (OCxGC)GAIN_2;
    ic[cic].tx_cfgb.oc3gc     = (OCxGC)GAIN_2;
    ic[cic].tx_cfgb.ocmode    = (OCMODE)OCMODE3_STATIC;
    ic[cic].tx_cfgb.ocax      = (OCABX)OCABX_ACTIVE_LOW;
    ic[cic].tx_cfgb.ocbx      = (OCABX)OCABX_ACTIVE_LOW;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //CFGBR5
  for(uint8_t cic = 0; cic < tIC; cic++)
  { 
    /* Init config B */
    ic[cic].tx_cfgb.diagsel   = (DIAGSEL)DIAGSEL7_OW_IxB_IAS_VBAT;
    ic[cic].tx_cfgb.gpio2eoc  = (GPIO2EOC)EOC_ENABLED;      
    ic[cic].tx_cfgb.gpio1c    = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio2c    = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio3c    = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio4c    = (GPIOxC)PULL_DOWN_OFF;   
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

  //------------------------------------------------------------------- ----//  
  // COMM
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    ic[cic].tx_comm.icomm[0]= 0x06; //Start communication
    ic[cic].tx_comm.icomm[1]= 0x00; //blank
    ic[cic].tx_comm.icomm[2]= 0x01; //Stop communication
    
    ic[cic].tx_comm.fcomm[0]= 0x00; //Master Acknowledge
    ic[cic].tx_comm.fcomm[1]= 0x00; //blank
    ic[cic].tx_comm.fcomm[2]= 0x09; //Stop communication
    
    ic[cic].tx_comm.data[0]= 0x55;
    ic[cic].tx_comm.data[1]= 0x55;
    ic[cic].tx_comm.data[2]= 0x55;
  }
  /* Write Config Data */
  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  // Delay for OCEN toggle
  Delay_us(OC_delay_8cycle_us);
  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);   

  printWriteConfig(tIC, &ic[0], Config, ALL_GRP);
  printReadConfig(tIC, &ic[0], Config, ALL_GRP);
#if 0
  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printWriteConfig(tIC, &ic[0], Config, B);
  printReadConfig(tIC, &ic[0], Config, B);
  printWriteConfig(tIC, &ic[0], Comm, NONE);
  printReadConfig(tIC, &ic[0], Comm, NONE);
#endif
}

/**
*******************************************************************************
* @brief Read Configuration Register A/B
*******************************************************************************
*/
void adi2950_read_config(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  /* Read Config Data */
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);   

  printReadConfig(tIC, &ic[0], Config, ALL_GRP);
  // test 
  // printReadConfig(tIC, &ic[0], Config, A);
  // printReadConfig(tIC, &ic[0], Config, B);
  // printReadConfig(tIC, &ic[0], Comm, NONE);
}

/**
*******************************************************************************
* @brief Start Adi1 Single Measurement
*******************************************************************************
*/
void adi2950_start_adi1_single_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;

  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI1 command with REDUNDANT_MEASUREMENT off and SingleShot and start timer for first conversion
  adBms2950_Adi1(tIC, &ic[0], RD_OFF, OPT0_SS, &command);
  Delay_ms(ADI1_delay_ms);
  // Wait for 5msec or send PLADC command to poll for the data.
#ifdef POLLING
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLADC);
#else
  Delay_ms(Polling_Delay_ms);
#endif

  printMsg("Adi1 conversion completed");
  printPollAdcConvTime(ic[0].pladc_count);
}

/**
*******************************************************************************
* @brief Start Adi1 Continuous Measurement
*******************************************************************************
*/
void adi2950_start_adi1_continuous_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;
  uint16_t index=0;
  uint8_t data[50];
  uint8_t size = 8;
  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI1 command with REDUNDANT_MEASUREMENT_ON for continuous mode without OW_WIRE_DETECTION
  adBms2950_Adi1(tIC, &ic[0], RD_ON, OPT8_C, &command);
  Delay_ms(ADI1_delay_ms); 
  // Delay_ms(ADI1_delay_ms); /* As per data sheet current register update rate(1ms) min  & average current register update rate (8ms) max*/

  for (loop_counter=0; loop_counter < ITER_LOOP_MEASUREMENT_COUNT; loop_counter++)
  {
    index = 0;
    
    // Poll on the I1PHA bits for the conversion completion as I1PHA bits changes from 0 to 3.
    do {
      adBmsRawWriteRead(tIC, &RFLAG[0], &data[index], size);
    } while(!((data[3] & 0x03)==0x03)); //Polling on I1PHA bits 
    do {
      adBmsRawWriteRead(tIC, &RFLAG[0], &data[index], size);
    } while(data[3]& 0x03); // It will come out when I1PHA become 0

    index = index+ (tIC * size) ;
    
    // Read Battery Current, Voltage and OC register values
#ifndef ADBMS2951    
    // Send RDI to read back the current values for I1ADC and I2ADC from DUT.
    adBmsReadData(tIC, &ic[0], RDI, Cr, NONE);
#endif  
    //Send RDVB to read back the voltage values for VB1ADC and VB2ADC from DUT.
    adBmsReadData(tIC, &ic[0], RDVB, Vbat, NONE); 
#ifndef ADBMS2951  
    // Send RDOC to read the OC values
    adBmsReadData(tIC, &ic[0], RDOC, Oc, NONE);
#endif  
    printCr(tIC, &ic[0]);
    printVbat(tIC, &ic[0]);
    printOc(tIC, &ic[0]);

    // CLRI to clear battery current registers for each loop    
    spiSendCmd(tIC, &ic[0], CLRI);
    
    // RDFLAG to read the Flag register
    adBmsReadData(tIC, &ic[0], RDFLAG, Flag, FLAG_NOERR);        
    printStatus(tIC, &ic[0], Flag, FLAG_NOERR);
  }   
  // ADI1 command with :CONT = 0:RD = 1 to stop the continous conversion.
  adBms2950_Adi1(tIC, &ic[0], RD_ON, OPT0_SS, &command);
  Delay_ms(ADI1_delay_ms);

  printMsg("Adi1 continuous conversion completed");
}

/**
*******************************************************************************
* @brief Start Adi2 Single Measurement
*******************************************************************************
*/
void adi2950_start_adi2_single_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;

  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI2 command in Singleshot mode
  adBms2950_Adi2(tIC, &ic[0], OPT0_SS, &command);
#ifdef POLLING    
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLADC);   
#else
  Delay_ms(Polling_Delay_ms);
#endif

  printMsg("Adi2 single conversion completed");
  printPollAdcConvTime(ic[0].pladc_count);
}

/**
*******************************************************************************
* @brief Start Adi2 Continuous Measurement
*******************************************************************************
*/
void adi2950_start_adi2_continuous_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;
  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI2 command with continuous mode and without diagnostic mode
  adBms2950_Adi2(tIC, &ic[0], OPT8_C, &command);
  Delay_ms(8); /* As per data sheet current register update rate(1ms) min  & average current register update rate (8ms) max */
  // Check IxCAL till both IxCALs = 1
  while((ic[0].state.i1cal == 0) || (ic[0].state.i2cal == 0))     
  { 
    // Send RDSTAT to read IxCALs in the STATUS register
    adBmsReadData(tIC, &ic[0], RDSTAT, Status, NONE);  
  }
  // Send ADI2 command with :CONT = 0 to stop the continous conversion.
  adBms2950_Adi2(tIC, &ic[0], OPT0_SS, &command);
  printMsg("Adi2 continuous conversion completed");
}

/**
*******************************************************************************
* @brief Start Adi1 Redundant Single Measurement
*******************************************************************************
*/
void adi2950_start_adi1_redundant_single_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;

  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI1 command with REDUNDANT_MEASUREMENT ON and SingleShot and start timer for first conversion
  adBms2950_Adi1(tIC, &ic[0], RD_ON, OPT0_SS, &command);
  Delay_ms(ADI1_delay_ms);
  // Wait for 5msec or send PLADC command to poll for the data.
#ifdef POLLING
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLADC);
#else
  Delay_ms(Polling_Delay_ms);
#endif

  printMsg("Adi1 redundant single conversion completed");
  printPollAdcConvTime(ic[0].pladc_count);
}

/**
*******************************************************************************
* @brief Start Adi1 Redundant Continuous Measurement
*******************************************************************************
*/
void adi2950_start_adi1_redundant_continuous_measurment(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;
  uint16_t index=0;
  uint8_t data[50];
  uint8_t size = 8;

  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send ADI1 command with REDUNDANT_MEASUREMENT_ON for continuous mode without OW_WIRE_DETECTION
  adBms2950_Adi1(tIC, &ic[0], RD_ON, OPT8_C, &command);
  // Delay_ms(8); /* As per data sheet current register update rate(1ms) min  & average current register update rate (8ms) max*/
  Delay_ms(ADI1_delay_ms);
  for (loop_counter=0; loop_counter < ITER_LOOP_MEASUREMENT_COUNT; loop_counter++)
  {
    index = 0;
    
    // Poll on the I1PHA bits for the conversion completion as I1PHA bits changes from 0 to 3.
    do {
      adBmsRawWriteRead(tIC, &RFLAG[0], &data[index], size);
    } while(!((data[3] & 0x03)==0x03)); //Polling on I1PHA bits 
    do {
      adBmsRawWriteRead(tIC, &RFLAG[0], &data[index], size);
    } while(data[3]& 0x03); // It will come out when I1PHA become 0

    index = index+ (tIC * size) ;
    
    // Read Battery Current, Voltage and OC register values
#ifndef ADBMS2951    
    // Send RDI to read back the current values for I1ADC and I2ADC from DUT.
    adBmsReadData(tIC, &ic[0], RDI, Cr, NONE);
#endif  
    //Send RDVB to read back the voltage values for VB1ADC and VB2ADC from DUT.
    adBmsReadData(tIC, &ic[0], RDVB, Vbat, NONE); 
#ifndef ADBMS2951  
    // Send RDOC to read the OC values
    adBmsReadData(tIC, &ic[0], RDOC, Oc, NONE);
#endif  
    printCr(tIC, &ic[0]);
    printVbat(tIC, &ic[0]);
    printOc(tIC, &ic[0]);

    // CLRI to clear battery current registers for each loop    
    spiSendCmd(tIC, &ic[0], CLRI);
    
    // RDFLAG to read the Flag register
    adBmsReadData(tIC, &ic[0], RDFLAG, Flag, FLAG_NOERR);        
    printStatus(tIC, &ic[0], Flag, FLAG_NOERR);
  }   

  // ADI1 command with :CONT = 0:RD = 1 to stop the continous conversion.
  adBms2950_Adi1(tIC, &ic[0], RD_ON, OPT0_SS, &command);
  Delay_ms(ADI1_delay_ms);

  printMsg("Adi1 redundant continuous conversion completed");
}

/**
*******************************************************************************
* @brief Read Current, Battery & Current and Battery Registers
*******************************************************************************
*/
void adi2950_read_cr_vbat_ivbat_registers(uint8_t tIC, cell_asic *ic)
{
  cmd_description command;
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDI, Cr, NONE);        /* Current Register Group */
  adBmsReadData(tIC, &ic[0], RDVB, Vbat, NONE);   /* Battery Voltage Group*/
  adBmsReadData(tIC, &ic[0], RDIVB1, Ivbat, NONE); /* Current and Battery Voltage Group */

  printCr(tIC, &ic[0]);
  printVbat(tIC, &ic[0]);
  printIvbat(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Read Overcurrent ADC Register
*******************************************************************************
*/
void adi2950_read_ocr_register(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDOC, Oc, NONE); /* Read OCxADC and REFADC results */
  printOc(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Read (Accumulated Current), (Accumulated Battery)  & (Accumulated Battery Current and Voltage) Registers
*******************************************************************************
*/
void adi2950_read_acccr_accvbat_accivbat_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDIACC, AccCr, NONE);
  adBmsReadData(tIC, &ic[0], RDVBACC, AccVbat, NONE);
  adBmsReadData(tIC, &ic[0], RDIVB1ACC, AccIvbat, NONE);

  printAccCr(tIC, &ic[0]);
  printAccVbat(tIC, &ic[0]);
  printAccIVbat(tIC, &ic[0]);
}

/**
*******************************************************************************
* @brief Read All Current & Battery Voltage Registers
*******************************************************************************
*/
void adi2950_all_current_battery_voltage_registers(uint8_t tIC, cell_asic *ic)
{
  // float voltage;
  // uint16_t temp;
  // cmd_description command;
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDI, Cr, NONE);           //Current Register Group
  adBmsReadData(tIC, &ic[0], RDVB, Vbat, NONE);      //Battery Voltage register group
  adBmsReadData(tIC, &ic[0], RDIVB1, Ivbat, NONE);    //Current and Battery votage register group

  adBmsReadData(tIC, &ic[0], RDIACC, AccCr, NONE);      //Accumulated Current Register group
  adBmsReadData(tIC, &ic[0], RDVBACC, AccVbat, NONE);   //Accumulated battery votage group
  adBmsReadData(tIC, &ic[0], RDIVB1ACC, AccIvbat, NONE); //Accumulated battery voltage and current register group

  adBmsReadData(tIC, &ic[0], RDOC, Oc, NONE);         // Overcurrent ADC register group

  printCr(tIC, &ic[0]);
  printVbat(tIC, &ic[0]);
  printIvbat(tIC, &ic[0]);
  printAccCr(tIC, &ic[0]);
  printAccVbat(tIC, &ic[0]);
  printAccIVbat(tIC, &ic[0]);
  printOc(tIC, &ic[0]);

  // CLRVX to clear all voltage registers (general purpose + aux)
  spiSendCmd(tIC, &ic[0], CLRVX);

  // ADV with OW = 0 and CH = 9 (All channels) and start timer for first conversion
  adBms2950_Adv(tIC, &ic[0], OW_OFF, RR_VCH0_VCH8);
#ifdef POLLING
  // Wait for 5msec/Send PLADC command to poll for the data.
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLV);
#else
  Delay_ms(Polling_Delay_ms);
#endif
#if 1
  // V1ADC register group
  adBmsReadData(tIC, &ic[0], RDV1A, GPV1, A); 
  adBmsReadData(tIC, &ic[0], RDV1B, GPV1, B); 
  adBmsReadData(tIC, &ic[0], RDV1C, GPV1, C); 
  adBmsReadData(tIC, &ic[0], RDV1D, GPV1, D);
  printVoltages(tIC, &ic[0], GPV1);
#else
  adBmsReadData(tIC, &ic[0], RDALLV, Rdallv, ALL_GRP);
  printRdallv(tIC, &ic[0]);
#endif
#if 1
  // V2ADC register group
  adBmsReadData(tIC, &ic[0], RDV2A, GPV2, A);
  adBmsReadData(tIC, &ic[0], RDV2B, GPV2, B);
  adBmsReadData(tIC, &ic[0], RDV2C, GPV2, C);
  adBmsReadData(tIC, &ic[0], RDV2D, GPV2, D);
  adBmsReadData(tIC, &ic[0], RDV2E, GPV2, E);
  printVoltages(tIC, &ic[0], GPV2);
#else
  adBmsReadData(tIC, &ic[0], RDALLR, Rdallr, ALL_GRP);
  printRdallr(tIC, &ic[0]);
#endif

  // ADX command for Aux voltage conversion and start timer for first conversion
  spiSendCmd(tIC, &ic[0], sADX);
#ifdef POLLING
  // PLX to poll on the conversion
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLX);
  // printf(" ic[0].pladc_count=%d ", ic[0].pladc_count);
#else
  Delay_ms(Polling_Delay_ms);
#endif

#if 1
  // Read Aux ADC Registers
  adBmsReadData(tIC, &ic[0], RDXA, Aux, A);
  adBmsReadData(tIC, &ic[0], RDXB, Aux, B);
  adBmsReadData(tIC, &ic[0], RDXC, Aux, C);    
  printAux(tIC, &ic[0], Aux, A);
  printAux(tIC, &ic[0], Aux, B);
  printAux(tIC, &ic[0], Aux, C);
#else
  adBmsReadData(tIC, &ic[0], RDALLX, Rdallx, ALL_GRP);
  printRdallx(tIC, &ic[0]);
#endif
  // CLRVX to clear all voltage registers (general purpose + aux)
  spiSendCmd(tIC, &ic[0], CLRVX);

}

/**
*******************************************************************************
* @brief Start Adv All Channel Measurement
*******************************************************************************
*/
void adi2950_start_adv_measurment(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBms2950_Adv(tIC, &ic[0], OW_OFF, RR_VCH0_VCH8);
#ifdef POLLING
  // Wait for 5msec/Send PLADC command to poll for the data.
  ic[0].pladc_count = adBmsPollAdc(tIC, &ic[0], PLV);
#else
  Delay_ms(Polling_Delay_ms);
#endif
  printMsg("Adv conversion completed");
  printPollAdcConvTime(ic[0].pladc_count);
}

/**
*******************************************************************************
* @brief Read Vr registers
*******************************************************************************
*/
void adi2950_read_vr_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  // V1ADC register group
  adBmsReadData(tIC, &ic[0], RDV1A, GPV1, A); 
  adBmsReadData(tIC, &ic[0], RDV1B, GPV1, B); 
  adBmsReadData(tIC, &ic[0], RDV1C, GPV1, C); 

  printVoltages(tIC, &ic[0], GPV1);
  // adBmsReadData(tIC, &ic[0], RDV1D, GPV1, D);

  // V2ADC register group
  // adBmsReadData(tIC, &ic[0], RDV2A, GPV2, A);
  // adBmsReadData(tIC, &ic[0], RDV2B, GPV2, B);
  adBmsReadData(tIC, &ic[0], RDV2C, GPV2, C);
  // adBmsReadData(tIC, &ic[0], RDV2D, GPV2, D);
  // adBmsReadData(tIC, &ic[0], RDV2E, GPV2, E);

  printVoltages(tIC, &ic[0], GPV2);
}

/**
*******************************************************************************
* @brief Read Redundant voltage registers
*******************************************************************************
*/
void adi2950_read_rvr_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  // Redundant voltage register group V1B ~ V6B
  adBmsReadData(tIC, &ic[0], RDV2A, GPV2, A);
  adBmsReadData(tIC, &ic[0], RDV2B, GPV2, B);

  printVoltages(tIC, &ic[0], GPV2);
}

/**
*******************************************************************************
* @brief Read voltage registers and Redundant voltage registers
*******************************************************************************
*/
void adi2950_read_vr_rvr_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDV1A, GPV1, A);
  adBmsReadData(tIC, &ic[0], RDV1B, GPV1, B);
  adBmsReadData(tIC, &ic[0], RDV1C, GPV1, C);
  adBmsReadData(tIC, &ic[0], RDV1D, GPV1, D);

  adBmsReadData(tIC, &ic[0], RDV2A, GPV2, A);
  adBmsReadData(tIC, &ic[0], RDV2B, GPV2, B);
  adBmsReadData(tIC, &ic[0], RDV2C, GPV2, C);
  adBmsReadData(tIC, &ic[0], RDV2D, GPV2, D);
  adBmsReadData(tIC, &ic[0], RDV2E, GPV2, E);
  
  printVoltages(tIC, &ic[0], GPV1);
  printVoltages(tIC, &ic[0], GPV2);
}

/**
*******************************************************************************
* @brief Read Vrx registers
*******************************************************************************
*/
void adi2950_read_vrx_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDXA, Aux, A);
  adBmsReadData(tIC, &ic[0], RDXB, Aux, B);
  adBmsReadData(tIC, &ic[0], RDXC, Aux, C);

  printAux(tIC, &ic[0], Aux, ALL_GRP);
}

/**
*******************************************************************************
* @brief Start Adaux Measurement
*******************************************************************************
*/
// replaced by spiSendCmd(tIC, &ic[0], sADX);
#if 0
void adi2950_start_adaux_measurment(uint8_t tIC)
{
  adBmsWakeupIc(tIC);
  adBms2950_Adaux(AUX_CH_TO_CONVERT);
  pladc_count = adBmsPollAdc(PLADC);
  printMsg("Adaux conversion completed");
  printPollAdcConvTime(pladc_count);
}
#endif

/**
*******************************************************************************
* @brief Read Adaux status A measurment
*******************************************************************************
*/
void adi2950_read_adaux_measurment(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  //adBmsReadData(tIC, &ic[0], RDSTATA, Status, A); /*!< Status A */
  adBmsReadData(tIC, &ic[0], RDXA, Status, A); /*!< Status A */
  printStatus(tIC, &ic[0], Status, A);
}

/**
*******************************************************************************
* @brief Read Status Register
*******************************************************************************
*/
void adi2950_read_status_register(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Status E */
  
  printStatus(tIC, &ic[0], Status, C); 
}

/**
*******************************************************************************
* @brief Read All Status Registers
*******************************************************************************
*/
void adi2950_read_all_status_registers(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    
  
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, NONE);
  // RDFLAG to read the OC related flags in the Flag register
  adBmsReadData(tIC, &ic[0], RDFLAG, Flag, FLAG_NOERR);

  printStatus(tIC, &ic[0], Status, ALL_GRP);
  printFlag(tIC, &ic[0], Flag);
}

/**
*******************************************************************************
* @brief Read Device SID
*******************************************************************************
*/
void adi2950_read_device_sid(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  printDeviceSID(tIC, &ic[0], SID);
}

/**
*******************************************************************************
* @brief Soft Reset
*******************************************************************************
*/
void adi2950_soft_reset(uint8_t tIC, cell_asic *ic)
{
  // 1. Wakeup
  adBmsWakeupIc(tIC);
  // 2. SRST
  spiSendCmd(tIC, &ic[0], SRST);   
  // 3. RSTCC
  adi2950_reset_cmd_count(tIC, &ic[0]);  

  printMsg("Soft Reset Done");
  Delay_ms(8); /*  After SRST cmd 8ms delay required*/
}

/**
*******************************************************************************
* @brief Reset command counter
*******************************************************************************
*/
void adi2950_reset_cmd_count(uint8_t tIC, cell_asic *ic)
{
  //Wakeup
  adBmsWakeupIc(tIC); 
  //SRST
  spiSendCmd(tIC, &ic[0], SRST);
  //RSTCC
  spiSendCmd(tIC, &ic[0], RSTCC); 
  for(int cic=0;cic< tIC;cic++)
  {
    ic[cic].cccrc.cal_cmd_cntr =0;
  }

  printMsg("Command Counter Reset Done");
}

/**
*******************************************************************************
* @brief Snapshot
*******************************************************************************
*/
void adi2950_snap(uint8_t tIC, cell_asic *ic)
{
  uint8_t SNAP_Delay = 1;

  // Wakeup
  adBmsWakeupIc(tIC);    

  // Send SNAP command to freeze the results.
  spiSendCmd(tIC, &ic[0], SNAP); 
  Delay_ms(SNAP_Delay); //need to check in runtime ???

  printMsg("Snap Done");
}

/**
*******************************************************************************
* @brief Release Snapshot
*******************************************************************************
*/
void adi2950_unsnap(uint8_t tIC, cell_asic *ic)
{
  uint8_t SNAP_Delay = 1;
  // Wakeup
  adBmsWakeupIc(tIC);    

  spiSendCmd(tIC, &ic[0], UNSNAP);
  Delay_ms(SNAP_Delay); 

  printMsg("Unsnap Done");
}

/**
*******************************************************************************
* @brief Set and reset the gpo pins(to drive output on gpo pins)
*******************************************************************************
*/
void adi2950_set_reset_gpo_pins(uint8_t tIC, cell_asic *ic)
{
  int option, gpo;
  printMsg("Please Enter: \n1:Set\n2:Reset\n");
  readUserInupt(&option);

  // Wakeup
  adBmsWakeupIc(tIC);    

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    // ic[cic].tx_cfga.refon = PWR_UP;
    if(option == 1)
    {
#ifdef MBED
      printf("IC[%d]: Select GPO Pin Drive to High\n", (cic+1));
      printf("0 : GPO1 \n");
      printf("1 : GPO2 \n");
      printf("2 : GPO3 \n");
      printf("3 : GPO4 \n");
      printf("4 : GPO5 \n");
      printf("5 : GPO6 \n");
      scanf("%d",&gpo);
#else
      printf("IC[%d]: Select GPO Pin Drive to High\n", (cic+1));
      printf("0 : GPO1 \n");
      printf("1 : GPO2 \n");
      printf("2 : GPO3 \n");
      printf("3 : GPO4 \n");
      printf("4 : GPO5 \n");
      printf("5 : GPO6 \n");
      scanf("%d",&gpo);
#endif
      // ic[cic].tx_cfga.gpo = adBms2950ConfigA_Gpo((GPO)gpo, GPO_SET);    /* Gpos pins drive to high */
      ic[cic].tx_cfga.gpo1c = PULLED_UP_TRISTATED;
      ic[cic].tx_cfga.gpo2c = PULLED_UP_TRISTATED;
      ic[cic].tx_cfga.gpo3c = PULLED_UP_TRISTATED;
      ic[cic].tx_cfga.gpo4c = PULLED_UP_TRISTATED;
      ic[cic].tx_cfga.gpo5c = PULLED_UP_TRISTATED;
      ic[cic].tx_cfga.gpo6c = PULLED_UP_TRISTATED;
#ifdef GPO3V3       
      ic[cic].tx_cfga.gpo1od = OPEN_DRAIN;
      ic[cic].tx_cfga.gpo2od = OPEN_DRAIN;
      ic[cic].tx_cfga.gpo3od = OPEN_DRAIN;
      ic[cic].tx_cfga.gpo4od = OPEN_DRAIN;
      ic[cic].tx_cfga.gpo5od = OPEN_DRAIN;
      ic[cic].tx_cfga.gpo6od = OPEN_DRAIN;
#else
      ic[cic].tx_cfga.gpo1od = PUSH_PULL;
      ic[cic].tx_cfga.gpo2od = PUSH_PULL;
      ic[cic].tx_cfga.gpo3od = PUSH_PULL;
      ic[cic].tx_cfga.gpo4od = PUSH_PULL;
      ic[cic].tx_cfga.gpo5od = PUSH_PULL;
      ic[cic].tx_cfga.gpo6od = PUSH_PULL;
#endif
    }
    else if(option == 2)
    {
#ifdef MBED
      printf("IC[%d]: Select GPO Pin Drive to Low\n", (cic+1));
      printf("0 : GPO1 \n");
      printf("1 : GPO2 \n");
      printf("2 : GPO3 \n");
      printf("3 : GPO4 \n");
      printf("4 : GPO5 \n");
      printf("5 : GPO6 \n");
      scanf("%d",&gpo);
#else
      printf("IC[%d]: Select GPO Pin Drive to Low\n", (cic+1));
      printf("0 : GPO1 \n");
      printf("1 : GPO2 \n");
      printf("2 : GPO3 \n");
      printf("3 : GPO4 \n");
      printf("4 : GPO5 \n");
      printf("5 : GPO6 \n");
      scanf("%d",&gpo);
#endif
      // ic[cic].tx_cfga.gpo = adBms2950ConfigA_Gpo((GPO)gpo, GPO_CLR);    /* Gpo pins drive to low*/
      ic[cic].tx_cfga.gpo1c = PULLED_DOWN;
      ic[cic].tx_cfga.gpo2c = PULLED_DOWN;
      ic[cic].tx_cfga.gpo3c = PULLED_DOWN;
      ic[cic].tx_cfga.gpo4c = PULLED_DOWN;
      ic[cic].tx_cfga.gpo5c = PULLED_DOWN;
      ic[cic].tx_cfga.gpo6c = PULLED_DOWN;  
    }
  }

  adBmsWakeupIc(tIC);
  
  // Send WRCFGA command with  GPO1 to GPO6 to 0
  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);

  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read status E for gpo pin status*/
  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printStatus(tIC, &ic[0], Status, E);
}

/**
*******************************************************************************
* @brief GPIO SPI Write to Slave.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_spi_write_to_slave(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    // Enable SPIM 3-wire mode
    ic[cic].tx_cfga.spi3w = (SPI3W)THREE_WIRE;

    // Set all GPIOs in tristate mode so that they cannot be overruled lo low state

    ic[cic].tx_comm.icomm[0] = 0x08;        // Generate a CSBM Low signal//0x08
    ic[cic].tx_comm.data[0]  = 0xFF;        // data1, write the data on COMM register (D0,D1,D2) for sending the data on SPI bus.
    ic[cic].tx_comm.fcomm[0] = 0x00;        // Holds CSBM low
    
    ic[cic].tx_comm.icomm[1] = 0x08;        // Generate a CSBM Low signal//0x08
    ic[cic].tx_comm.data[1]  = 0xFF;        // data2
    ic[cic].tx_comm.fcomm[1] = 0x09;        // CSBM high
    
    ic[cic].tx_comm.icomm[2] = 0x0F;        // No transmit//0x0F
    ic[cic].tx_comm.data[2]  = 0xFF;        // data3
    ic[cic].tx_comm.fcomm[2] = 0x09;        // CSBM high   
  }

  adBmsWakeupIc(tIC);

  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read Status E for gpio status*/

  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  adBms2950_Stcomm(tIC, &ic[0]);
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);

  printMsg("GPIO SPI Write to Slave Completed");
  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printStatus(tIC, &ic[0], Status, E);
  printWriteCommData(tIC, &ic[0], Comm);
  printReadCommData(tIC, &ic[0], Comm);
}

/**
*******************************************************************************
* @brief GPIO SPI Read from Slave.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_spi_read_from_slave(uint8_t tIC, cell_asic *ic)
{
  // Wakeup
  adBmsWakeupIc(tIC);    
  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /*!< All gpios pull down disable */
    // Send WRCFGB with GPIOxC = 1 (x = 1, 2, 3, 4) 
    ic[cic].tx_cfgb.gpio1c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio2c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio3c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio4c = (GPIOxC)PULL_DOWN_OFF; 
    adBmsWriteData(tIC, &ic[cic], WRCFGB, Config, B);   

    ic[cic].tx_comm.icomm[0] = 0x8;        /*!< Generate a CSBM Low signal */
    ic[cic].tx_comm.fcomm[0] = 0x0;        /*!< Holds CSBM low */
    ic[cic].tx_comm.icomm[1] = 0x8;        /*!< Generate a CSBM Low signal */
    ic[cic].tx_comm.fcomm[1] = 0x9;        /*!< CSBM high */
    ic[cic].tx_comm.icomm[2] = 0xF;        /*!< No transmit */
    ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< CSBM high */

    ic[cic].tx_comm.data[0] = 0x55;        /*!< data1, write the data on COMM register (D0,D1,D2) for sending the data on SPI bus*/
    ic[cic].tx_comm.data[1] = 0xAA;        /*!< data2 */
    ic[cic].tx_comm.data[2] = 0xFF;        /*!< data3 */
  }
  adBmsWakeupIc(tIC);

  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read Status E for gpio status*/
  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  
  adBms2950_Stcomm(tIC, &ic[0]);
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);
  
  printMsg("GPIO SPI Read from Slave Completed");
  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printStatus(tIC, &ic[0], Status, E);
  printWriteCommData(tIC, &ic[0], Comm);
  printReadCommData(tIC, &ic[0], Comm);
}

/**
*******************************************************************************
* @brief GPIO I2C Write on the GPIO Port.
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_i2c_write_to_slave(uint8_t tIC, cell_asic *ic)
{
  uint8_t slave_data =0x13;

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /*! All gpios pull down disable*/
    // Send WRCFGB with GPIOxC = 1 (x = 1, 2, 3, 4) 
    ic[cic].tx_cfgb.gpio1c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio2c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio3c = (GPIOxC)PULL_DOWN_OFF;
    ic[cic].tx_cfgb.gpio4c = (GPIOxC)PULL_DOWN_OFF; 
    // adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
    // // Send RDCFGB to read configuration registers
    // adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   

    //I2C Write Sequence    
    ic[cic].tx_comm.icomm[0] = 0x6;        // Generate a START Signal on I2C Port Followed by Data Transmission
    ic[cic].tx_comm.fcomm[0] = 0x8;        // Master Generated an NACK Signal
    ic[cic].tx_comm.icomm[1] = 0x0;        // Blank, SDA Held Low Between Bytes
    ic[cic].tx_comm.fcomm[1] = 0x8;        // Master Generated an NACK Signal
    ic[cic].tx_comm.icomm[2] = 0x0;        // Blank, SDA Held Low Between Bytes
    ic[cic].tx_comm.fcomm[2] = 0x9;        // Master Generated a NACK Signal, Master Generated a STOP Signal
    
    ic[cic].tx_comm.data[0] = 0xA0;        //data1
    ic[cic].tx_comm.data[1] = 0x00;        //data2
    ic[cic].tx_comm.data[2] = slave_data;  //data3
  }
  // Wakeup
  adBmsWakeupIc(tIC);    

  // adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  // adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  // adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read Status E for gpio status*/
  
  adBmsWriteData(tIC, &ic[0], WRCFGB, Config, B);
  // Send RDCFGB to read configuration registers
  adBmsReadData(tIC, &ic[0], RDCFGB, Config, B);   
  // Send WRCOMM with icomm and fcomm and data bytes (I2C write sequence)to write data to slave
  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  // Send STCOMM command to initiate communication
  adBms2950_Stcomm(tIC, &ic[0]);
  // Send RDCOMM command to read the comm register
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);
  
  printMsg("GPIO I2C Write data to Slave completed");
  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printWriteConfig(tIC, &ic[0], Config, B);
  printReadConfig(tIC, &ic[0], Config, B);
  printStatus(tIC, &ic[0], Status, E);
  printWriteCommData(tIC, &ic[0], Comm);
  printReadCommData(tIC, &ic[0], Comm);
}

/**
*******************************************************************************
* @brief GPIO I2C Read from the GPIO Ports(using eeprom 24AA01)
*        Refer to the data sheet.
*******************************************************************************
*/
void adi2950_gpio_i2c_read_from_slave(uint8_t tIC, cell_asic *ic)
{

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /*!< All gpios pull down disable*/
    //I2C read Sequence   
    ic[cic].tx_comm.icomm[0] = 0x6;        // Generate a START Signal on I2C Port Followed by Data Transmission
    ic[cic].tx_comm.fcomm[0] = 0x8;        // Master Generated an NACK Signal
    ic[cic].tx_comm.icomm[1] = 0x0;        // Blank, SDA Held Low Between Bytes
    ic[cic].tx_comm.fcomm[1] = 0x8;        // Master Generated an NACK Signal
    ic[cic].tx_comm.icomm[2] = 0x6;        // Blank, SDA Held Low Between Bytes
    ic[cic].tx_comm.fcomm[2] = 0x8;        // Master Generated a NACK Signal, Master Generated a STOP Signal
    
    ic[cic].tx_comm.data[0] = 0xA0;        //data1
    ic[cic].tx_comm.data[1] = 0x00;        //data2
    ic[cic].tx_comm.data[2] = 0xA1;        //data3    
  }
  // Wakeup
  adBmsWakeupIc(tIC);    

  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read Status E for gpio status*/

  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  adBms2950_Stcomm(tIC, &ic[0]);
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);

  printWriteConfig(tIC, &ic[0], Config, A);
  printReadConfig(tIC, &ic[0], Config, A);
  printWriteCommData(tIC, &ic[0], Comm);
  printStatus(tIC, &ic[0], Status, E);

  for(uint8_t cic = 0; cic < tIC; cic++)
  {
    /*!< All gpios pull down disable */

    ic[cic].tx_comm.icomm[0] = 0x0;        /*!< SDA held low */
    ic[cic].tx_comm.fcomm[0] = 0x9;        /*!<Slave NACk+ master STOP */
    ic[cic].tx_comm.icomm[1] = 0x7;        /*!< SDA held high */
    ic[cic].tx_comm.fcomm[1] = 0x9;        /*!< Slave NACk+ master STOP */
    ic[cic].tx_comm.icomm[2] = 0x7;        /*!<SDA held high */
    ic[cic].tx_comm.fcomm[2] = 0x9;        /*!< Slave NACk+ master STOP */

    ic[cic].tx_comm.data[0] = 0xFF;        /*!<data1 */
    ic[cic].tx_comm.data[1] = 0xFF;        /*!<data2 */
    ic[cic].tx_comm.data[2] = 0xFF;        /*!<data3 */
  }
  /***************************** Read from slave devcie *****************/
  adBmsWakeupIc(tIC);

  adBmsWriteData(tIC, &ic[0], WRCFGA, Config, A);
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, E); /*!< Read Status E for gpio status*/
  // WRCOMM with icomm and fcomm and data bytes to read data from slave
  adBmsWriteData(tIC, &ic[0], WRCOMM, Comm, NONE);
  // STCOMM command to initiate communication
  adBms2950_Stcomm(tIC, &ic[0]);
  // RDCOMM to read the configurtaion registers
  adBmsReadData(tIC, &ic[0], RDCOMM, Comm, NONE);
  
  printMsg("GPIO I2C Read data from Slave completed");
  printReadCommData(tIC, &ic[0], Comm);
  printStatus(tIC, &ic[0], Status, E);
}

/**
*******************************************************************************
* @brief Soft Reset
*******************************************************************************
*/
void soft_reset(uint8_t tIC, cell_asic *ic)
{
  adBmsWakeupIc(tIC);
  spiSendCmd(tIC, ic, SRST);
  Delay_ms(8); /*  After SRST cmd 8ms delay required*/
}

/** @}*/
/** @}*/