/**
********************************************************************************
*
* @file:    print_result.h
*
* @brief:   This file contains the print result.
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

#ifndef __RESULT_H
#define __RESULT_H

#include "adbms2950.h"

void printWriteConfig(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp);
void printReadConfig(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp);
void printDeviceSID(uint8_t tIC, cell_asic *IC, TYPE type);
void printWriteCommData(uint8_t tIC, cell_asic *IC, TYPE type);
void printReadCommData(uint8_t tIC, cell_asic *IC, TYPE type);
void printCr(uint8_t tIC, cell_asic *IC);
void printVoltages(uint8_t tIC, cell_asic *IC, TYPE type);
void printVbat(uint8_t tIC, cell_asic *IC);
void printIvbat(uint8_t tIC, cell_asic *IC);
void printAccVbat(uint8_t tIC, cell_asic *IC);
void printAccIVbat(uint8_t tIC, cell_asic *IC);
void printAccCr(uint8_t tIC, cell_asic *IC);
void printOc(uint8_t tIC, cell_asic *IC);
void printRdallv(uint8_t tIC, cell_asic *IC); 
void printRdallr(uint8_t tIC, cell_asic *IC);
void printRdallx(uint8_t tIC, cell_asic *IC);
void printGpV1(uint8_t tIC, cell_asic *IC);
void printStatus(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp);
void printFlag(uint8_t tIC, cell_asic *IC, TYPE type);
void printAux(uint8_t tIC, cell_asic *IC, TYPE type, GRP grp);
void printMenu(void);
void printMsg(char *msg);
void printPollAdcConvTime(int count);
void printResultCount(int count);
void readUserInupt(int *user_command);
float getVoltage(int data);
float getCurrent(uint32_t data);
float getAccCurrent(uint32_t data);
float getAccVbat(uint32_t data);
float getOverCurrent(uint8_t data);

#endif /* __RESULT_H */
/** @}*/
/** @}*/