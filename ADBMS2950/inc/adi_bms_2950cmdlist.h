/**
********************************************************************************
*
* @file:    adi_bms_2950cmdlist.h
*
* @brief:   ADBMS2950 command list header file.
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
/*! \addtogroup BMS_Driver
*  @{
*/

/*! \addtogroup Command_List
*  @{
*/

#ifndef __ADBMSCOMMAND_H
#define __ADBMSCOMMAND_H

/*============= I N C L U D E S =============*/
/*============== D E F I N E S ===============*/
/*============= E X T E R N A L S ============*/
/*============= E N U M E R A T O R S ============*/

#include "common.h"

/*!< configuration registers commands */
extern uint8_t WRCFGA[2];
extern uint8_t WRCFGB[2];
extern uint8_t RDCFGA[2];
extern uint8_t RDCFGB[2];

/*!< Read cell voltage result registers commands */
extern uint8_t RDI[2];
extern uint8_t RDVB[2];
extern uint8_t RDIVB1[2];

/* Read VxADC result registers commands */
extern uint8_t RDV1A[2];
extern uint8_t RDV1B[2];
extern uint8_t RDV1C[2];
extern uint8_t RDV1D[2];
extern uint8_t RDV2A[2];//RDRVA
extern uint8_t RDV2B[2];
extern uint8_t RDV2C[2];
extern uint8_t RDV2D[2];
extern uint8_t RDV2E[2];

/* Read OCxADC result registers commands */
extern uint8_t RDOC[2];

/* Read all commands */
extern uint8_t RDALLI[2];
extern uint8_t RDALLA[2];
extern uint8_t RDALLC[2];
extern uint8_t RDALLV[2];
extern uint8_t RDALLR[2];
extern uint8_t RDALLX[2];

/*!< Read Result Registers Commands B */
extern uint8_t RDIACC[2];
extern uint8_t RDVBACC[2];
extern uint8_t RDIVB1ACC[2];
extern uint8_t RDALLB[2];

/* Read Result Registers Commands C */
extern uint8_t RDCALL[2];

extern uint8_t RDSTAT[2];
extern uint8_t RDFLAG[2];
extern uint8_t RDFLAGERR[2];   /* ERR */

/*!< Read all Status Registers */
extern uint8_t RDASALL[2];

/*!< Pwm registers commands */
extern uint8_t WRPWM[2];
extern uint8_t RDPWM[2];
extern uint8_t WRPWM2[2];
extern uint8_t RDPWM2[2];

/*!< Clear commands */
extern uint8_t CLRI[2];
extern uint8_t CLRA[2];
extern uint8_t CLRO[2];
extern uint8_t CLRC[2];
extern uint8_t CLRVX [2];  
extern uint8_t CLRFLAG[2];

/*!< Poll adc command */
extern uint8_t PLADC[2];
extern uint8_t PLI1[2];
extern uint8_t PLI2[2];
extern uint8_t PLV[2];
extern uint8_t PLX[2];

/*!< GPIOs Comm commands */
extern uint8_t WRCOMM[2];
extern uint8_t RDCOMM[2];
extern uint8_t STCOMM[13];

/*!< Control Commands */
extern uint8_t RDSID[2];
extern uint8_t RSTCC[2];
extern uint8_t SNAP[2];
extern uint8_t UNSNAP[2];
extern uint8_t SRST[2];

//Command + PEC
extern uint8_t RSTATD[4];
extern uint8_t RFLAG[4];
extern uint8_t sRDI[4];
extern uint8_t sRSTATA [4];
extern uint8_t sRDVA [4];
extern uint8_t sRDVB [4];
extern uint8_t sRDVC [4];
extern uint8_t sRDVD [4];
extern uint8_t sRDIAV[4];
extern uint8_t sRDVBAT[4];
extern uint8_t sADI1[2];
extern uint8_t sADX[2];

/* Testmode and debugging commands */
extern uint8_t TM_48[2];

/* Read Aux ADC result registers commands */
extern uint8_t RDXA[2];
extern uint8_t RDXB[2];
extern uint8_t RDXC[2];

#endif /* __BMS_COMMAND_H */

/** @}*/
/** @}*/
