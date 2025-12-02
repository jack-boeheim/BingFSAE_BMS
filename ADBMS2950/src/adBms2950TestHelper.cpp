/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    adBms2950TestHelper.c
* @brief:   Test Helper functions
* @version: $Revision$
* @date:    $Date$
*****************************************************************************/

#include "adbms_2950.h"
#include "pal.h"
#include "adBms2950TestHelper.h"

/**
*******************************************************************************
* Function: adBms2950_Functional_header
* @brief Header for functional test cases
*
* @details This function has the implementation of header.
* Steps:
* 1.  Send CS toggle to wakeup the IC
* 2.  Send SRST command with a delay to ensure the soft reset             
* 3.  Reset command counter.
* 4.  Read CFGA until REFUP bit is high
* 5.  Send RDSID to read SID
* 6.  Send RDSTAT to read REV ID 
* 7.  Send CLRFLAG to clear flag register
* 8.  Send RDFLAG to read flag register
*
* Parameters:
* @param [in]	tIC             Total IC
*
* @param [in]  *IC             cell_asic stucture pointer
*
* @return None
*
*******************************************************************************
*/
void adBms2950_Functional_header(uint8_t tIC, cell_asic *ic)
{
  uint8_t cic = 0;

  // 1. Wakeup
  adBmsWakeupIc(tIC);

  // 2. SRST
  spiSendCmd(tIC, &ic[0], SRST);   

  // 3. RSTCC
  adi2950_reset_cmd_count(tIC, &ic[0]);  
  // Send RDCFGA to read back reset values of CFGA
  adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);  

  // 4. REFUP bit check; Read CFGA
  while (ic[0].rx_cfga.refup != 1)
  {
    // Send RDCFGA to read back REFUP in CFGA
    adBmsReadData(tIC, &ic[0], RDCFGA, Config, A);      
  }

  // 5. Read SID
  adBmsReadData(tIC, &ic[0], RDSID, SID, NONE);

  // 6. Status Rev ID
  adBmsReadData(tIC, &ic[0], RDSTAT, Status, NONE);

  // 7. Send CLRFLAG to Flag
  adBms2950_Configure_CLRflag_Data(cic, tIC, &ic[0]);   
  adBmsWriteData(tIC, &ic[0], CLRFLAG, Clrflag, NONE);

  // 8. Read Flag  - all bits should be cleared
  adBmsReadData(tIC, &ic[0], RDFLAG, Flag, FLAG_NOERR);

}

/**
*******************************************************************************
* Function: adBms2950_read_status_ABCDE
* @brief Helper function to read status A B C D E
*
* @details This function has the implementation of header.
*
* Parameters:
* @param [in]	tIC             Total IC
*
* @param [in]  *IC             cell_asic stucture pointer
*
* @param [in]  *log_array      Log array data pointer
*
*******************************************************************************
*/
void adBms2950_Configure_CLRflag_Data(uint8_t cic, uint8_t tIC, cell_asic *ic)
{
      ic[cic].clflag.vdruv              = CL_FLAG_SET;
      ic[cic].clflag.ocmm               = CL_FLAG_SET;
      ic[cic].clflag.oc3l               = CL_FLAG_SET;
      ic[cic].clflag.ocagd_clrm         = CL_FLAG_SET;
      ic[cic].clflag.ocal               = CL_FLAG_SET;
      ic[cic].clflag.oc1l               = CL_FLAG_SET;
      
      ic[cic].clflag.vdduv              = CL_FLAG_SET;
      ic[cic].clflag.noclk              = CL_FLAG_SET;
      ic[cic].clflag.refflt             = CL_FLAG_SET;
      ic[cic].clflag.ocbgd              = CL_FLAG_SET;
      ic[cic].clflag.ocbl               = CL_FLAG_SET;
      ic[cic].clflag.oc2l               = CL_FLAG_SET;
      
      ic[cic].clflag.vregov             = CL_FLAG_SET;
      ic[cic].clflag.vreguv             = CL_FLAG_SET;
      ic[cic].clflag.vdigov             = CL_FLAG_SET;
      ic[cic].clflag.vdiguv             = CL_FLAG_SET;
      ic[cic].clflag.sed1               = CL_FLAG_SET;
      ic[cic].clflag.med1               = CL_FLAG_SET;
      ic[cic].clflag.sed2               = CL_FLAG_SET;
      ic[cic].clflag.med2               = CL_FLAG_SET;
      
      ic[cic].clflag.vdel               = CL_FLAG_SET;
      ic[cic].clflag.vde                = CL_FLAG_SET;
      ic[cic].clflag.spiflt             = CL_FLAG_SET;
      ic[cic].clflag.reset              = CL_FLAG_SET;
      ic[cic].clflag.thsd               = CL_FLAG_SET;
      ic[cic].clflag.tmode              = CL_FLAG_SET;
      ic[cic].clflag.oscflt             = CL_FLAG_SET;
}

/** @}*/
/** @}*/
