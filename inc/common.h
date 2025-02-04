/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
******************************************************************************
* @file:    common.h
* @brief:   Standard common header files
* @version: $Revision$
* @date:    $Date$
* Developed by: ADIBMS Software team, Bangalore, India
*****************************************************************************/
#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MBED  /* Switch between IAR Workbench or Mbed Studio. */
#define MBED5 /* Define for use with MBED5 (Nucleo F207GZ), otherwise comment out for MBED6 (Nucleo G474RE) */


#ifdef MBED
#include "mbed.h"
#include "SPI.h"
#include "Timer.h"
#endif

#endif