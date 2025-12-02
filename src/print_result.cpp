/**
********************************************************************************
*
* @file:    print_result.c
*
* @brief:   This file contains the print result function implementation.
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

/*! \addtogroup Print_Result
*  @{
*/

#include "application.h"
#ifdef MBED
#define printVal printf
//extern Serial pc;
#else /* not MBED - IAR, ... */
#define printVal printf
#endif


/**
*******************************************************************************
* Function: printWriteConfig
* @brief Print write config A/B result.
*
* @details This function Print write config result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *IC      cell_asic stucture pointer
*
* @param [in]  type     Enum type of resistor
*
* @param [in]  grp      Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
void printWriteConfig(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    if(type == Config)
    {
      if(grp == A)
      {
        printVal("Write Config A:\n");
        printVal("0x%X, ", IC[ic].configa.tx_data[0]);
        printVal("0x%X, ", IC[ic].configa.tx_data[1]);
        printVal("0x%X, ", IC[ic].configa.tx_data[2]);
        printVal("0x%X, ", IC[ic].configa.tx_data[3]);
        printVal("0x%X, ", IC[ic].configa.tx_data[4]);
        printVal("0x%X\n\n", IC[ic].configa.tx_data[5]);
      }
      else if(grp == B)
      {
        printVal("Write Config B:\n");
        printVal("0x%X, ", IC[ic].configb.tx_data[0]);
        printVal("0x%X, ", IC[ic].configb.tx_data[1]);
        printVal("0x%X, ", IC[ic].configb.tx_data[2]);
        printVal("0x%X, ", IC[ic].configb.tx_data[3]);
        printVal("0x%X, ", IC[ic].configb.tx_data[4]);
        printVal("0x%X\n\n", IC[ic].configb.tx_data[5]);
      }
      else if(grp == ALL_GRP)
      {
        printVal("Write Config A:\n");
        printVal("0x%X, ", IC[ic].configa.tx_data[0]);
        printVal("0x%X, ", IC[ic].configa.tx_data[1]);
        printVal("0x%X, ", IC[ic].configa.tx_data[2]);
        printVal("0x%X, ", IC[ic].configa.tx_data[3]);
        printVal("0x%X, ", IC[ic].configa.tx_data[4]);
        printVal("0x%X\n\n", IC[ic].configa.tx_data[5]);

        printVal("Write Config B:\n");
        printVal("0x%X, ", IC[ic].configb.tx_data[0]);
        printVal("0x%X, ", IC[ic].configb.tx_data[1]);
        printVal("0x%X, ", IC[ic].configb.tx_data[2]);
        printVal("0x%X, ", IC[ic].configb.tx_data[3]);
        printVal("0x%X, ", IC[ic].configb.tx_data[4]);
        printVal("0x%X\n\n", IC[ic].configb.tx_data[5]);

        printVal("Write Config Comm:\n");
        printVal("ICOM:\n");
        printVal("0x%X, ", IC[ic].tx_comm.icomm[0]);
        printVal("0x%X, ", IC[ic].tx_comm.icomm[1]);
        printVal("0x%X, \n", IC[ic].tx_comm.icomm[2]);
        printVal("FCOM:\n");
        printVal("0x%X, ", IC[ic].tx_comm.fcomm[0]);
        printVal("0x%X, ", IC[ic].tx_comm.fcomm[1]);
        printVal("0x%X\n", IC[ic].tx_comm.fcomm[2]);
        printVal("DATA:\n");
        printVal("0x%X, ", IC[ic].tx_comm.data[0]);
        printVal("0x%X, ", IC[ic].tx_comm.data[1]);
        printVal("0x%X, \n\n", IC[ic].tx_comm.data[2]);
    
      }
      else{ printVal("Wrong Register Group Select\n"); }
    }
  }
}

/**
*******************************************************************************
* Function: printReadConfig
* @brief Print Read config A/B result.
*
* @details This function Print read config result into IAR I/O terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @param [in]  TYPE     Enum type of resistor
*
* @param [in]  GRP      Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
void printReadConfig(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    if(type == Config)
    {
      if(grp == A)
      {
        printVal("Read Config A:\n");
        printVal("0x%X, ", IC[ic].configa.rx_data[0]);
        printVal("0x%X, ", IC[ic].configa.rx_data[1]);
        printVal("0x%X, ", IC[ic].configa.rx_data[2]);
        printVal("0x%X, ", IC[ic].configa.rx_data[3]);
        printVal("0x%X, ", IC[ic].configa.rx_data[4]);
        printVal("0x%X, ", IC[ic].configa.rx_data[5]);
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.cfgr_pec);
      }
      else if(grp == B)
      {
        printVal("Read Config B:\n");
        printVal("0x%X, ", IC[ic].configb.rx_data[0]);
        printVal("0x%X, ", IC[ic].configb.rx_data[1]);
        printVal("0x%X, ", IC[ic].configb.rx_data[2]);
        printVal("0x%X, ", IC[ic].configb.rx_data[3]);
        printVal("0x%X, ", IC[ic].configb.rx_data[4]);
        printVal("0x%X, ", IC[ic].configb.rx_data[5]);
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.cfgr_pec);
      }
      else if(grp == ALL_GRP)
      {
        printVal("Read Config A:\n");
        printVal("0x%X, ", IC[ic].configa.rx_data[0]);
        printVal("0x%X, ", IC[ic].configa.rx_data[1]);
        printVal("0x%X, ", IC[ic].configa.rx_data[2]);
        printVal("0x%X, ", IC[ic].configa.rx_data[3]);
        printVal("0x%X, ", IC[ic].configa.rx_data[4]);
        printVal("0x%X, \n", IC[ic].configa.rx_data[5]);
        // printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        // printVal("PECError:%d\n",IC[ic].cccrc.cfgr_pec);

        printVal("Read Config B:\n");
        printVal("0x%X, ", IC[ic].configb.rx_data[0]);
        printVal("0x%X, ", IC[ic].configb.rx_data[1]);
        printVal("0x%X, ", IC[ic].configb.rx_data[2]);
        printVal("0x%X, ", IC[ic].configb.rx_data[3]);
        printVal("0x%X, ", IC[ic].configb.rx_data[4]);
        printVal("0x%X, \n", IC[ic].configb.rx_data[5]);
        // printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        // printVal("PECError:%d\n\n",IC[ic].cccrc.cfgr_pec);

        printVal("Read Config Comm:\n");
        printVal("ICOM:\n");
        printVal("0x%X, ", IC[ic].rx_comm.icomm[0]);
        printVal("0x%X, ", IC[ic].rx_comm.icomm[1]);
        printVal("0x%X, \n", IC[ic].rx_comm.icomm[2]);
        printVal("FCOM:\n");
        printVal("0x%X, ", IC[ic].rx_comm.fcomm[0]);
        printVal("0x%X, ", IC[ic].rx_comm.fcomm[1]);
        printVal("0x%X\n", IC[ic].rx_comm.fcomm[2]);
        printVal("DATA:\n");
        printVal("0x%X, ", IC[ic].rx_comm.data[0]);
        printVal("0x%X, ", IC[ic].rx_comm.data[1]);
        printVal("0x%X, ", IC[ic].rx_comm.data[2]);

        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.cfgr_pec);
      }
      else{ printVal("Wrong Register Group Select\n"); }
    }
  }
}

/**
*******************************************************************************
* Function: PrintDeviceSID
* @brief Print Device SID.
*
* @details This function Print Device SID into IAR I/O terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *IC      cell_asic stucture pointer
*
* @param [in]  type     Enum type of resistor
*
* @return None
*
*******************************************************************************
*/
void printDeviceSID(uint8_t tIC, cell_asic *IC, TYPE type)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    if(type == SID)
    {
      printVal("Read Device SID:\n");
      printVal("0x%X, ", IC[ic].sid.sid[0]);
      printVal("0x%X, ", IC[ic].sid.sid[1]);
      printVal("0x%X, ", IC[ic].sid.sid[2]);
      printVal("0x%X, ", IC[ic].sid.sid[3]);
      printVal("0x%X, ", IC[ic].sid.sid[4]);
      printVal("0x%X, ", IC[ic].sid.sid[5]);
      printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d\n\n",IC[ic].cccrc.sid_pec);
    }
    else{ printVal("Wrong Register Type Select\n"); }
  }
}

/**
*******************************************************************************
* Function: printWriteCommData
* @brief Print Write Comm data.
*
* @details This function Print write comm data.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *IC      cell_asic stucture pointer
*
* @param [in]  type     Enum type of resistor
*
* @return None
*
*******************************************************************************
*/
void printWriteCommData(uint8_t tIC, cell_asic *IC, TYPE type)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    if(type == Comm)
    {
      printVal("Write Comm Data:\n");
      printVal("0x%X, ", IC[ic].com.tx_data[0]);
      printVal("0x%X, ", IC[ic].com.tx_data[1]);
      printVal("0x%X, ", IC[ic].com.tx_data[2]);
      printVal("0x%X, ", IC[ic].com.tx_data[3]);
      printVal("0x%X, ", IC[ic].com.tx_data[4]);
      printVal("0x%X\n\n", IC[ic].com.tx_data[5]);
    }
    else{ printVal("Wrong Register Group Select\n"); }
  }
}

/**
*******************************************************************************
* Function: printReadCommData
* @brief Print Read Comm Data.
*
* @details This function print read comm data.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *IC      cell_asic stucture pointer
*
* @param [in]  type     Enum type of resistor
*
* @return None
*
*******************************************************************************
*/
void printReadCommData(uint8_t tIC, cell_asic *IC, TYPE type)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    if(type == Comm)
    {
      printVal("Read Comm Data:\n");
      printVal("ICOM0:0x%X, ", IC[ic].rx_comm.icomm[0]);
      printVal("ICOM1:0x%X, ", IC[ic].rx_comm.icomm[1]);
      printVal("ICOM2:0x%X\n", IC[ic].rx_comm.icomm[2]);
      printVal("FCOM0:0x%X, ", IC[ic].rx_comm.fcomm[0]);
      printVal("FCOM1:0x%X, ", IC[ic].rx_comm.fcomm[1]);
      printVal("FCOM2:0x%X\n", IC[ic].rx_comm.fcomm[2]);
      printVal("DATA0:0x%X, ", IC[ic].rx_comm.data[0]);
      printVal("DATA1:0x%X, ", IC[ic].rx_comm.data[1]);
      printVal("DATA2:0x%X\n", IC[ic].rx_comm.data[2]);
      printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d\n\n",IC[ic].cccrc.comm_pec);
    }
    else{ printVal("Wrong Register Type Select\n"); }
  }
}

/**
*******************************************************************************
* Function: printCr
* @brief Print Current Result.
*
* @details This function Print current result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printCr(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read Current:\n");
    printVal("I1(V):%f, ", getCurrent(IC[ic].i.i1));
    printVal("I2(V):%f\n", getCurrent(IC[ic].i.i2));
    printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.cr_pec);
  }
}

/**
*******************************************************************************
* Function: printVoltages
* @brief Print Voltages.
*
* @details This function Print Voltages into terminal.
*
* Parameters:
* @param [in]   tIC    Total IC
*
* @param [in]  *IC    cell_asic stucture pointer
*
* @param [in]  type    Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
void printVoltages(uint8_t tIC, cell_asic *IC, TYPE type)
{
  float voltage;
  uint16_t temp;
  uint8_t channel;

  if((type == GPV1)) { channel = VR_SIZE; }
  else if (type == GPV2) { channel = RVR_SIZE; }

  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n", (ic+1));
    if(type == GPV1)
    {
      for(uint8_t index = 0; index < 8; index++)
      {
        temp = IC[ic].vr.v_codes[index];
        voltage = getVoltage(temp);
        printVal("V%dA = %7.4fV, ", (index+1), voltage); 
      } 
      printVal("VREF2A = %7.4fV, ", getVoltage(IC[ic].vr.v_codes[8])); 
      printVal("V7A = %7.4fV, ", getVoltage(IC[ic].vr.v_codes[9])); 
      printVal("V8A = %7.4fV, ", getVoltage(IC[ic].vr.v_codes[10])); 
      printVal("V9B = %7.4fV, ", getVoltage(IC[ic].vr.v_codes[11])); 
      printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d", IC[ic].cccrc.vr_pec);
    } /* if(type == GPV1)*/
    else if(type == GPV2)
    {
      for(uint8_t index = 0; index < 6; index++)
      {
        temp = IC[ic].rvr.redv_codes[index];
        voltage = getVoltage(temp);
        printVal("V%dB = %7.4fV, ", (index+1), voltage); 
      }
      printVal("V9B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[6])); 
      printVal("V10B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[7])); 
      printVal("VREF2B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[8])); 
      printVal("V10B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[9])); 
      printVal("VREF2A = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[10])); 
      printVal("VREF2B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[11])); 
      printVal("V10B = %7.4fV, ", getVoltage(IC[ic].rvr.redv_codes[12])); 
      printVal("CCount:%d,", IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d", IC[ic].cccrc.rvr_pec);
    } /* (type == GPV2) */
    else { printVal("Wrong Register Group Select\n"); }
    printVal("\n\n");
  } /* for(... ic < tIC ...) */
}

/**
*******************************************************************************
* Function: printVbat
* @brief Print VBAT Result.
*
* @details This function Print the VBAT result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printVbat(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read VBAT:\n");
    printVal("VBAT1: %f, ", getVoltage(IC[ic].vbat.vbat1));
    printVal("VBAT2: %f, ", getVoltage(IC[ic].vbat.vbat2));
    printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.vbat_pec);
  }
}

/**
*******************************************************************************
* Function: printIvbat
* @brief Print IVBAT result.
*
* @details This function Print the IVBAT result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printIvbat(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read IVBAT:\n");
    printVal("I1: %f, ", getCurrent(IC[ic].ivbat.i1));
    printVal("VBAT1: %f, ", getVoltage(IC[ic].ivbat.vbat1));
    printVal("CCount: %d, ",IC[ic].cccrc.cmd_cntr);
    printVal("PECError: %d\n\n",IC[ic].cccrc.ivbat_pec);
  }
}

/**
*******************************************************************************
* Function: printAccVbat
* @brief Print VBATACC result.
*
* @details This function Print the accumulated result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printAccVbat(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read AccVbat:\n");
    printVal("VB1ACC :%fV, ", getAccVbat(IC[ic].vbacc.vb1acc));
    printVal("VB2ACC: %fV, ", getAccVbat(IC[ic].vbacc.vb2acc));
    printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.avgvbat_pec);
  }
}

/**
*******************************************************************************
* Function: printAccIVbat
* @brief Print ACCIVBAT result.
*
* @details This function Print the accivbat result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printAccIVbat(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read AccIVbat:\n");
    printVal("I1ACC: %f, ", getAccCurrent(IC[ic].i_vbacc.i1acc));
    printVal("VB1ACC: %fV, ", getAccVbat(IC[ic].i_vbacc.vb1acc));
    printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.avgivbat_pec);
  }
}

/**
*******************************************************************************
* Function: printAccCr
* @brief Print IACC result.
*
* @details This function Print the accumulated result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printAccCr(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:\n",(ic+1));
    printVal("Read AccCr:\n");
    printVal("I1ACC: %fmV, ", getAccCurrent(IC[ic].iacc.i1acc));
    printVal("I2ACC: %fmV, ", getAccCurrent(IC[ic].iacc.i2acc));
    printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.avgcr_pec);
  }
}

/**
*******************************************************************************
* Function: printOc
* @brief Print OC result.
*
* @details This function Print the oc result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printOc(uint8_t tIC, cell_asic *IC)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("Read OCR:\n");
    printVal("OC1R: %f(mV), ", getOverCurrent(IC[ic].oc.oc1r));
    printVal("OC2R: %f(mV), ", getOverCurrent(IC[ic].oc.oc2r));
    printVal("OC3R: %f(mV), ", getOverCurrent(IC[ic].oc.oc3r));
    printVal("REFR: %f(mV), ", getOverCurrent(IC[ic].oc.refr));
    printVal("OC3MAX: %f(mV), ", getOverCurrent(IC[ic].oc.oc3max));
    printVal("OC3MIN: %f(mV), ", getOverCurrent(IC[ic].oc.oc3min));
    printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
    printVal("PECError:%d\n\n",IC[ic].cccrc.oc_pec);
  }
}

/**
*******************************************************************************
* Function: printRdallv
* @brief Print RDALLV result.
*
* @details This function Print the RDALLV result into terminal.
*
* Parameters:
* @param [in]   tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printRdallv(uint8_t tIC, cell_asic *IC)
{
  float voltage;

  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n",(ic+1));
    voltage = getVoltage(IC[ic].rdallv.v1);
    printVal("V1A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v2);
    printVal("V2A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v3);
    printVal("V3A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v4);
    printVal("V4A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v5);
    printVal("V5A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v6);
    printVal("V6A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v7);
    printVal("V7A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v8);
    printVal("V8A = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v9);
    printVal("V9B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallv.v10);
    printVal("V10B = %7.4fV \n", voltage);
  }
  printVal("\n\n");
}

/**
*******************************************************************************
* Function: printRdallr
* @brief Print RDALLR result.
*
* @details This function Print the RDALLR result into terminal.
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printRdallr(uint8_t tIC, cell_asic *IC)
{
  float voltage;

  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n",(ic+1));
    voltage = getVoltage(IC[ic].rdallr.v1);
    printVal("V1B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallr.v2);
    printVal("V2B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallr.v3);
    printVal("V3B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallr.v4);
    printVal("V4B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallr.v5);
    printVal("V5B = %7.4fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallr.v6);
    printVal("V6B = %7.4fV \n", voltage);
    // voltage = getVoltage(IC[ic].rdallr.v7);
    // printVal("V7B = %7.4fV \n", voltage);
    // voltage = getVoltage(IC[ic].rdallr.v8);
    // printVal("V8B = %7.4fV \n", voltage);
    // voltage = getVoltage(IC[ic].rdallr.v9);
    // printVal("V9B = %7.4fV \n", voltage);
    // voltage = getVoltage(IC[ic].rdallr.v10);
    // printVal("V10B = %7.4fV \n", voltage);
  }
  printVal("\n\n");
}

/**
*******************************************************************************
* Function: printRdallx
* @brief Print RDALLX result.
*
* @details This function Print the RDALLX result into terminal.
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printRdallx(uint8_t tIC, cell_asic *IC)
{
  float voltage;

  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n",(ic+1));
    voltage = getVoltage(IC[ic].rdallx.vref2A);
    printVal("VREF2A = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.vref2B);
    printVal("VREF2B = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.vref1p25);
    printVal("VREF1P25 = %fV \n", voltage);
    printVal("TMP1 = %fC \n", ((IC[ic].rdallx.tmp1 / 61.8) - 250));
    voltage = getVoltage(IC[ic].rdallx.vreg);
    printVal("VREG = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.vdd);
    printVal("VDD = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.vdig);
    printVal("VDIG = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.epad);
    printVal("EPAD = %fV \n", voltage);
    voltage = getVoltage(IC[ic].rdallx.vdiv);
    printVal("VDIV = %fV \n", voltage);
    printVal("TMP2 = %fC \n", ((IC[ic].rdallx.tmp2 / 20.5)-267));
  }
  printVal("\n\n");
}

/**
*******************************************************************************
* Function: printGpV1
* @brief Print GPV1 result.
*
* @details This function Print the GPV1 result into terminal.
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printGpV1(uint8_t tIC, cell_asic *IC)
{
  float voltage;
  uint16_t temp;
  uint8_t channel;

  channel = VR_SIZE;
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n",(ic+1));
    for(uint8_t index = 0; index < channel; index++)
    {
      temp = IC[ic].vr.v_codes[index];
      voltage = getVoltage(temp);
      if(index == 8) { printVal("VREF2A = %7.4fV \n", voltage); }
      else { printVal("V%2d = %7.4fV \n", (index+1), voltage); }
      if(index == (channel-1))
      {
        printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d",IC[ic].cccrc.vr_pec);
      }
    }
  }
  printVal("\n\n");
}

/**
*******************************************************************************
* Function: printGpV2
* @brief Print GPV2 result.
*
* @details This function Print the GPV2 result into terminal.
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void printGpV2(uint8_t tIC, cell_asic *IC)
{
  float voltage;
  uint16_t temp;
  uint8_t channel;

  channel = RVR_SIZE;
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d: \n",(ic+1));
    for(uint8_t index = 0; index < channel; index++)
    {
      temp = IC[ic].rvr.redv_codes[index];
      voltage = getVoltage(temp);
      if(index == 8) { printVal("VREF2B = %7.4fV \n", voltage); }
      else if(index == 10) { printVal("VREF2A = %7.4fV \n", voltage); }
      else if(index == 11) { printVal("VREF2B = %7.4fV \n", voltage); }
      else { printVal("V%2d = %7.4fV \n", (index+1), voltage); }
      if(index == (channel-1))
      {
        printVal("CCount:%d,",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d",IC[ic].cccrc.vr_pec);
      }
    }
  }
  printVal("\n\n");
}

/**
 *******************************************************************************
 * Function: printAux
 * @brief Print Aux ADC reg. result.
 *
 * @details This function Print Aux ADC reg. result into terminal.
 *
 * Parameters:
 * @param [in]	tIC      Total IC
 *
 * @param [in]  *IC      cell_asic stucture pointer
 *
 * @param [in]  type     Enum type of resistor
 *
 * @param [in]  grp      Enum type of resistor group
 *
 * @return None
 *
 *******************************************************************************
*/
#if 0
void printAux(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp)
{
  float voltage;
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:",(ic+1));
    if(type == Aux)
    {
      if(grp == A)
      {
        printVal("Aux A:\n");
        voltage = getVoltage(IC[ic].auxa.vref1p25);
        printVal("VREF1P25:%fV, ", voltage);
        // printVal("TMP1:%fC, ", (((IC[ic].auxa.tmp1 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling //Sayani
        printVal("TMP1:%fC, ", ((IC[ic].auxa.tmp1 / 61.8) - 250));
        voltage = getVoltage(IC[ic].auxa.vreg); 
        // printVal("VREG:%fV\n", (voltage + 1.5));
        printVal("VREG:%fV, ", (voltage * 0.24)); // voltage = VREG * 240microV
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.aux_pec);
      }
      else if(grp == B)
      {
        printVal("Aux B:\n");
        voltage = getVoltage(IC[ic].auxb.vdd);
        printVal("VDD:%fV, ", voltage); // voltage = VDD * 1mV
        voltage = getVoltage(IC[ic].auxb.vdig);
        // printVal("VDIG:%fV, ", voltage);
        printVal("VDIG:%fV, ", (voltage * 0.24)); // voltage = VDIG * 240microV
        voltage = getVoltage(IC[ic].auxb.epad);
        printVal("EPAD:%fV, ", voltage); // voltage = EPAD * 100microV
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.aux_pec);
      }
      else if(grp == C)
      {
        printVal("Aux C:\n");
        voltage = getVoltage(IC[ic].auxc.vdiv);
        printVal("VDIV:%fV, ", voltage); // voltage = VDIV * 100microV
        // printVal("TMP2:%fC, ", (((IC[ic].auxc.tmp2 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling//Sayani
        printVal("TMP2:%fC, ", ((IC[ic].auxc.tmp2)/20.5)-267); 
        printVal("OSCCNT:0x%X, ", IC[ic].auxc.osccnt);
        printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n", IC[ic].cccrc.aux_pec);
      }
      else if(grp == ALL_GRP)
      {
        printVal("Aux A:\n");
        voltage = getVoltage(IC[ic].auxa.vref1p25);
        printVal("VREF1P25:%fV, ", voltage); // voltage = VREF1P25 * 100microV
        // printVal("TMP1:%fC, ", (((IC[ic].auxa.tmp1 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling//Sayani
        printVal("TMP1:%fC, ", ((IC[ic].auxa.tmp1 / 61.8) - 250));
        voltage = getVoltage(IC[ic].auxa.vreg);
        // printVal("VREG:%fV\n", (voltage + 1.5));
        printVal("VREG:%fV\n", (voltage * 0.24)); // voltage = VREG * 240microV
        
        printVal("Aux B:\n");
        voltage = getVoltage(IC[ic].auxb.vdd);
        printVal("VDD:%fV, ", voltage); // voltage = VDD * 1mV
        voltage = getVoltage(IC[ic].auxb.vdig);
        // printVal("VDIG:%fV, ", voltage);        
        printVal("VDIG:%fV, ", (voltage * 0.24)); // voltage = VDIG * 240microV
        voltage = getVoltage(IC[ic].auxb.epad);
        printVal("EPAD:%fV\n", voltage); // voltage = EPAD * 100microV
        
        printVal("Aux C:\n");
        voltage = getVoltage(IC[ic].auxc.vdiv);
        printVal("VDIV:%fV, ", voltage); // voltage = VDIV * 100microV
        // printVal("TMP2:%fC, ", (((IC[ic].auxc.tmp2 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling//Sayani
        printVal("TMP2:%fC, ", ((IC[ic].auxc.tmp2)/20.5)-267); 
        printVal("OSCCNT:0x%X, ", IC[ic].auxc.osccnt);
        
        printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n", IC[ic].cccrc.aux_pec);
      }
      else{ printVal("Wrong Register Group Select\n"); }
    }
  }
}
#endif

/**
 *******************************************************************************
 * Function: PrintFlag
 * @brief Print flag reg. result.
 *
 * @details This function Print flag result into terminal.
 *
 * Parameters:
 * @param [in]	tIC      Total IC
 *
 * @param [in]  *IC      cell_asic stucture pointer
 *
 * @param [in]  type     Enum type of resistor
 *
 * @param [in]  grp      Enum type of resistor group
 *
 * @return None
 *
 *******************************************************************************
*/
void printFlag(uint8_t tIC, cell_asic *IC, TYPE type)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:",(ic+1));
    if(type == Flag)
    {
      printVal("\t Flag:");
#if 1    // to switch between bit wise/byte wise extraction of STATC values     
      printVal("OC1L      :0x%X, ", IC[ic].flag.oc1l);
      printVal("OC2L      :0x%X, ", IC[ic].flag.oc2l);
      printVal("OC3L      :0x%X, ", IC[ic].flag.oc3l);
      printVal("OCAL      :0x%X, ", IC[ic].flag.ocal);
      printVal("OCBL      :0x%X, ", IC[ic].flag.ocbl);
      printVal("OCAGD_CLRM:0x%X, ", IC[ic].flag.ocagd_clrm);
      printVal("OCBGD     :0x%X, ", IC[ic].flag.ocbgd);
      printVal("OCMM      :0x%X, ", IC[ic].flag.ocmm);      
      
      printVal("I1CNT     :0x%X, ", IC[ic].flag.i1cnt);
      printVal("I2CNT     :0x%X, ", IC[ic].flag.i2cnt);
      printVal("I1PHA     :0x%X, ", IC[ic].flag.i1pha);
      
      printVal("VDRUV     :0x%X, ", IC[ic].flag.vdruv);
      printVal("VDDUV     :0x%X, ", IC[ic].flag.vdduv);
      printVal("VREGOV    :0x%X, ", IC[ic].flag.vregov);
      printVal("VREGUV    :0x%X, ", IC[ic].flag.vreguv);
      printVal("VDIGOV    :0x%X, ", IC[ic].flag.vdigov);
      printVal("VDIGUV    :0x%X, ", IC[ic].flag.vdiguv);
      
      printVal("V1D       :0x%X, ", IC[ic].flag.v1d);
      printVal("V2D       :0x%X, ", IC[ic].flag.v2d);
      printVal("I1D       :0x%X, ", IC[ic].flag.i1d);
      printVal("I2D       :0x%X, ", IC[ic].flag.i2d); 
      
      printVal("VDE       :0x%X, ", IC[ic].flag.vde);
      printVal("VDE1      :0x%X, ", IC[ic].flag.vdel);
           
      printVal("SED1      :0x%X, ", IC[ic].flag.sed1);
      printVal("SED2      :0x%X, ", IC[ic].flag.sed2);
      printVal("MED1      :0x%X, ", IC[ic].flag.med1);
      printVal("MED2      :0x%X, ", IC[ic].flag.med2);      

      printVal("NOCLK     :0x%X, ", IC[ic].flag.noclk);
      printVal("SPIFLT    :0x%X, ", IC[ic].flag.spiflt);
      printVal("RESET     :0x%X, ", IC[ic].flag.reset);
      printVal("THSD      :0x%X, ", IC[ic].flag.thsd);
      printVal("TMODE     :0x%X, ", IC[ic].flag.tmode);
      printVal("OSCFLT    :0x%X, ", IC[ic].flag.oscflt);
      printVal("REFFLT    :0x%X\n", IC[ic].flag.refflt);
      
      printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d\n\n", IC[ic].cccrc.flag_pec);
#else         
      printVal("0x%X, ", IC[ic].flg.rx_data[0]);
      printVal("0x%X, ", IC[ic].flg.rx_data[1]);
      printVal("0x%X, ", IC[ic].flg.rx_data[2]);
      printVal("0x%X, ", IC[ic].flg.rx_data[3]);
      printVal("0x%X, ", IC[ic].flg.rx_data[4]);
      printVal("0x%X\n\n", IC[ic].flg.rx_data[5]);
#endif 
    }
  }
}

/**
*******************************************************************************
* Function: PrintStatus
* @brief Print status reg. result.
*
* @details This function Print status result into terminal.
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *IC      cell_asic stucture pointer
*
* @param [in]  type     Enum type of resistor
*
* @param [in]  grp      Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
void printStatus(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp)
{
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:",(ic+1));
    if(type == Status)
    {
      printVal("Status :\n");
      
      printVal("OCAP     :0x%X, ", IC[ic].state.ocap );
      printVal("OCBP     :0x%X, ", IC[ic].state.ocbp );
      printVal("DER      :0x%X\n", IC[ic].state.der  );
      printVal("I1CAL    :0x%X, ", IC[ic].state.i1cal);
      printVal("I2CAL    :0x%X, ", IC[ic].state.i2cal);
      
      printVal("GPO1L    :0x%X\n", IC[ic].state.gpo1l);
      printVal("GPO2L    :0x%X, ", IC[ic].state.gpo2l);
      printVal("GPO3L    :0x%X, ", IC[ic].state.gpo3l);
      printVal("GPO4L    :0x%X\n", IC[ic].state.gpo4l);
      printVal("GPO5L    :0x%X, ", IC[ic].state.gpo5l);
      printVal("GPO6L    :0x%X, ", IC[ic].state.gpo6l);
      
      printVal("GPO1H    :0x%X\n", IC[ic].state.gpo1h);
      printVal("GPO2H    :0x%X, ", IC[ic].state.gpo2h);
      printVal("GPO3H    :0x%X, ", IC[ic].state.gpo3h);
      printVal("GPO4H    :0x%X\n", IC[ic].state.gpo4h);
      printVal("GPO5H    :0x%X, ", IC[ic].state.gpo5h);
      printVal("GPO6H    :0x%X, ", IC[ic].state.gpo6h);
      
      printVal("GPIO1L   :0x%X, ", IC[ic].state.gpio1l);
      printVal("GPIO2L   :0x%X\n", IC[ic].state.gpio2l);
      printVal("GPIO3L   :0x%X, ", IC[ic].state.gpio3l);
      printVal("GPIO4L   :0x%X, ", IC[ic].state.gpio4l);
      
      printVal("REV      :0x%X\n", IC[ic].state.rev);    
    
      printVal("CCount    :%d, ",  IC[ic].cccrc.cmd_cntr);
      printVal("PECError:%d\n\n",  IC[ic].cccrc.stat_pec);   
    }
  }
}

/**
 *******************************************************************************
 * Function: printAux
 * @brief Print Aux ADC reg. result.
 *
 * @details This function Print Aux ADC reg. result into terminal.
 *
 * Parameters:
 * @param [in]	tIC      Total IC
 *
 * @param [in]  *IC      cell_asic stucture pointer
 *
 * @param [in]  type     Enum type of resistor
 *
 * @param [in]  grp      Enum type of resistor group
 *
 * @return None
 *
 *******************************************************************************
*/
void printAux(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp)
{
  float voltage;
  for(uint8_t ic = 0; ic < tIC; ic++)
  {
    printVal("IC%d:",(ic+1));
    if(type == Aux)
    {
      if(grp == A)
      {
        printVal("Aux A:\n");
        voltage = getVoltage(IC[ic].auxa.vref1p25);
        printVal("VREF1P25:%fV, ", voltage);
        // printVal("TMP1:%fC, ", (((IC[ic].auxa.tmp1 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling 
        printVal("TMP1:%fC, ", ((IC[ic].auxa.tmp1 / 61.8) - 250));
        voltage = getVoltage(IC[ic].auxa.vreg); 
        printVal("VREG:%fV, ", (voltage * 0.24)); // voltage = VREG * 240microV
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.aux_pec);
      }
      else if(grp == B)
      {
        printVal("Aux B:\n");
        voltage = getVoltage(IC[ic].auxb.vdd);
        printVal("VDD:%fV, ", voltage); // voltage = VDD * 1mV
        voltage = getVoltage(IC[ic].auxb.vdig);
        printVal("VDIG:%fV, ", (voltage * 0.24)); // voltage = VDIG * 240microV
        voltage = getVoltage(IC[ic].auxb.epad);
        printVal("EPAD:%fV, ", voltage); // voltage = EPAD * 100microV
        printVal("CCount:%d, ",IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n",IC[ic].cccrc.aux_pec);
      }
      else if(grp == C)
      {
        printVal("Aux C:\n");
        voltage = getVoltage(IC[ic].auxc.vdiv);
        printVal("VDIV:%fV, ", voltage); // voltage = VDIV * 100microV
        // printVal("TMP2:%fC, ", (((IC[ic].auxc.tmp2 * 150e-6 )+ 1.5)/0.0075)-273);//check scaling
        printVal("TMP2:%fC, ", ((IC[ic].auxc.tmp2)/20.5)-267); 
        printVal("OSCCNT:0x%X, ", IC[ic].auxc.osccnt);
        printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n", IC[ic].cccrc.aux_pec);
      }
      else if(grp == ALL_GRP)
      {
        printVal("Aux A:\n");
        voltage = getVoltage(IC[ic].auxa.vref1p25);
        printVal("VREF1P25:%fV, ", voltage); // voltage = VREF1P25 * 100microV
        printVal("TMP1:%fC, ", ((IC[ic].auxa.tmp1 / 61.8) - 250));
        voltage = getVoltage(IC[ic].auxa.vreg);
        printVal("VREG:%fV\n", (voltage * 0.24)); // voltage = VREG * 240microV
        
        printVal("Aux B:\n");
        voltage = getVoltage(IC[ic].auxb.vdd);
        printVal("VDD:%fV, ", voltage); // voltage = VDD * 1mV
        voltage = getVoltage(IC[ic].auxb.vdig);
        printVal("VDIG:%fV, ", (voltage * 0.24)); // voltage = VDIG * 240microV
        voltage = getVoltage(IC[ic].auxb.epad);
        printVal("EPAD:%fV\n", voltage); // voltage = EPAD * 100microV
        
        printVal("Aux C:\n");
        voltage = getVoltage(IC[ic].auxc.vdiv);
        printVal("VDIV:%fV, ", voltage); // voltage = VDIV * 100microV
        printVal("TMP2:%fC, ", ((IC[ic].auxc.tmp2)/20.5)-267); 
        printVal("OSCCNT:0x%X, ", IC[ic].auxc.osccnt);
        
        printVal("CCount:%d, ", IC[ic].cccrc.cmd_cntr);
        printVal("PECError:%d\n\n", IC[ic].cccrc.aux_pec);
      }
      else{ printVal("Wrong Register Group Select\n"); }
    }
  }
}

/**
*******************************************************************************
* Function: printMenu
* @brief Print Command Menu.
*
* @details This function print all command menu.
*
* @return None
*
*******************************************************************************
*/
void printMenu()
{
  printVal("List of ADBMS2950 Command:\n");
  printVal("Write and Read Configuration: 1                              |Read Device SID: 26                 \n");
  printVal("Read Configuration: 2                                        |Soft Reset: 27                      \n");
  printVal("Start ADI1 Conversion(Single Shot): 3                        |Reset cmd counter: 28               \n");
  printVal("Start ADI2 Conversion(Single Shot): 4                        |SNAP(Stop Reg. updates): 29         \n");
  printVal("Start ADI1 Conversion(Continuous): 5                         |UNSNAP(Resume Reg. updates): 30     \n");
  printVal("Start ADI2 Conversion(Continuous): 6                         |Set Reset GPO Pins: 31              \n");
  printVal("Start ADI1 Redundant Conversion(Single Shot): 7              |GPIO SPI Write to Slave: 32         \n");
  printVal("Start ADI1 Redundant Conversion(Continuous): 8               |GPIO SPI Read from Slave: 33        \n");
  printVal("Read CR, VBAT & IVBAT Registers(Single Shot): 9              |GPIO I2C Write to Slave: 34         \n");
  printVal("Read CR, VBAT & IVBAT Registers(Continuous): 10              |GPIO I2C Read from Slave: 35        \n");
  printVal("Read Overcurrent ADC Register(Single Shot): 11               |                                    \n");
  printVal("Read Overcurrent ADC Register(Continuous): 12                |                                    \n");
  printVal("Read Accumulated CR, VBAT & IVBAT Registers(Single Shot): 13     |                                    \n");
  printVal("Read Accumulated CR, VBAT & IVBAT Registers(Continuous): 14      |                                    \n");
  printVal("Read All CR and VBAT Voltage Registers(Single Shot): 15      |                                    \n");
  printVal("Read All CR and VBAT Voltage Registers(Continuous): 16       |                                    \n");
  printVal("Start ADV All Channel Conversion: 17                         |                                    \n");
  printVal("Read All VR Registers(RDV commands): 18                      |                                    \n");
  printVal("Read All RVR Registers(RDRVA & RDRVB commands): 19           |                                    \n");
  printVal("Read All VR & RVR Registers(RDV, RDRVA & RDRVB commands): 20 |                                    \n");
  printVal("Read Voltage Registers(RDAUXC & RDAUXD commands)(NA): 21     |                                    \n");
  printVal("Start ADAUX All Channel Conversion: 22                       |                                    \n");
  printVal("Read ADAUX Measurement: 23                                   |                                    \n");
  printVal("Read Status C Register: 24                                   |                                    \n");
  printVal("Read All Status Registers: 25                                |                                    \n");

  printVal("\n");
  printVal("Print '0' for menu\n");
  printVal("Please enter command: \n");
  printVal("\n\n");
}

/**
*******************************************************************************
* Function: printMsg
* @brief Print Message.
*
* @details This function print message into terminal.
* Parameters:
* @param [in]   msg    Message string
*
* @return None
*
*******************************************************************************
*/
void printMsg(char *msg)
{
#ifdef MBED
  printf("%s\n", msg);
#else /* IAR */
  printf("%s\n", msg);
#endif /* MBED */
}

/**
*******************************************************************************
* Function: printPollAdcConvTime
* @brief Print Poll adc conversion Time.
*
* @details This function print poll adc conversion Time.
*
* @return None
*
*******************************************************************************
*/
void printPollAdcConvTime(int count)
{
#ifdef MBED
  printf("Adc Conversion Time = %fms\n", (float)(count/1000.0));
#else /* IAR */
  printf("Adc Conversion Time = %fms\n", (float)(count/64000.0));
#endif /* MBED */
}

/**
*******************************************************************************
* Function: printResultCount
* @brief Print Result Count.
*
* @details This function print the continuous measurment result count.
*
* @return None
*
*******************************************************************************
*/
void printResultCount(int count)
{
#ifdef MBED
  printf("Result Count:%d\n", (count+1));
#else /* IAR */
  printf("Result Count:%d\n", (count+1));
#endif /* MBED */
}

/**
*******************************************************************************
* Function: readUserInupt
* @brief Read command input & print into console.
*
* @details This function print the input command & print into console.
*
* @return None
*
*******************************************************************************
*/
void readUserInupt(int *user_command)
{
#ifdef MBED
    scanf("%d", user_command);
    printf("Entered cmd:%d\n", *user_command);
#else /* IAR */
    scanf("%d", user_command);
    printf("Entered cmd:%d\n", *user_command);
#endif /* MBED */
}

/**
*******************************************************************************
* Function: getVoltage
* @brief Get Voltage with multiplication factor.
*
* @details This function calculates the voltage.
*
* Parameters:
* @param [in]   data    Register value(uint16_t)
*
* @return Voltage(float)
*
*******************************************************************************
*/
float getVoltage(int data)
{
  float voltage;
  voltage = 100e-6 * (int16_t)data; /* Interpreting as 16-bit to be sure of length so signed works */
  return voltage;
}

/**
*******************************************************************************
* Function: getCurrent
* @brief Get Current with multiplication factor.
*
* @details This function calculates the current.
*
* Parameters:
* @param [in]   data    Register value(uint32_t)
*
* @return Current(float)
*
*******************************************************************************
*/
float getCurrent(uint32_t data)
{
  float current;
  // current = 1e-6 * (((int32_t)data << (32-24)) >> (32-24));
  current = 1e-6 * ((int32_t)(data << (32-24)) >> (32-24));
  // current = 1e-6 * ((int32_t)(data));
  return current;
}

/**
*******************************************************************************
* Function: getAccCurrent
* @brief Get Accumulated Current with multiplication factor.
*
* @details This function calculates the current.
*
* Parameters:
* @param [in]   data    Register value(uint32_t)
*
* @return Current(float)
*
*******************************************************************************
*/
float getAccCurrent(uint32_t data)
{
  float current;
  current = 1e-6 * 0.125 * ((int32_t)(data << (32-24)) >> (32-24));
  // current = 1e-6 * 0.125 * ((int32_t)(data << (32-18)) >> (32-18));
  return current;
}

/**
*******************************************************************************
* Function: getAccVbat
* @brief Get Accumulated VBAT with multiplication factor.
*
* @details This function calculates the acc vbat.
*
* Parameters:
* @param [in]   data    Register value(uint32_t)
*
* @return Current(float)
*
*******************************************************************************
*/
float getAccVbat(uint32_t data)
{
  float accvbat;
  accvbat = 100e-6 * 0.125 * ((int32_t)(data << (32-24)) >> (32-24));
  return accvbat;
}

/**
*******************************************************************************
* Function: getOverCurrent
* @brief Get Over Current with multiplication factor.
*
* @details This function calculates the over current.
*
* Parameters:
* @param [in]   data    Register value(uint8_t)
*
* @return Current(float)
*
*******************************************************************************
*/
float getOverCurrent(uint8_t data)
{
  float over_current;
//  over_current = 0.005 * ((int8_t)(data << (8-7)) >> (8-7));
  over_current =4.78e-3 * ((int8_t)(data)+0.5);
  return over_current;
}

/** @}*/
/** @}*/