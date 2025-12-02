/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    adBms2950TestHelper.h
* @brief:   helper function.
* @version: $Revision$
* @date:    $Date$
*****************************************************************************/
#ifndef __TESTHELPER_H
#define __TESTHELPER_H

//#include "mcuWrapper.h"
//#include "main.h"
#include "common.h"

#define I1CNT_DEFAULT (uint16_t)0

/*!
*  \enum Enum for selecting polling function input.
* : DMM Measurement Type.
*/
// typedef enum
// {
//     I1CNT_Default,
//     I1CNT_Latest
// } I1CNT_Input;

void adBms2950_Functional_header(uint8_t tIC, cell_asic *ic);
void adBms2950_Configure_CLRflag_Data(uint8_t cic, uint8_t tIC, cell_asic *ic);

#endif  /* __TESTHHELPER_H */