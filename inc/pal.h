/**
********************************************************************************
*
* @file:    pal.h
*
* @brief:   This file contains the pal header files & functions.
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

/*! \addtogroup Platform_Abstracion_Layer
*  @{
*/

#ifndef __PAL_H
#define __PAL_H

/*============= I N C L U D E S =============*/
/*============== D E F I N E S ===============*/
/*============= E X T E R N A L S ============*/
/*============= E N U M E R A T O R S ============*/

#include "common.h"

#include <cstdint>
#include "application.h"

#ifdef MBED

//MBED doesnt seem to be defined so pulling to above ^
#include <cstdint>
#include "application.h"

#else   /* not MBED */

// #include "application.h"
// #include "stm32f4xx_hal.h"
// #include "stm32f4xx_hal_tim.h"
// #include "stm32f4xx_it.h"
// #include "adbms_main.h"
// #include "stm32f4xx_hal_gpio.h"

#endif 	/* MBED */

void Delay_ms(uint32_t delay);
void Delay_us(uint32_t us);
void adBmsCsLow(void);
void adBmsCsHigh(void);
void adBmsWakeupIc(uint8_t total_ic);
void spiWriteBytes(uint8_t *tx_data, uint16_t size);
void spiWriteReadBytes(uint8_t *tx_data, uint8_t *rx_data, uint16_t size);
void spiReadBytes(uint8_t *rx_data, uint16_t size);
void adBmsRawWriteRead(uint8_t total_ic, uint8_t *tx_data, uint8_t *rx_data, uint8_t size);
void startTimer();
void stopTimer();
uint32_t getTimCount();
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState); //Taken from mcuFunctions.h
void spiTransmit(SPI_HandleTypeDef *hspi,uint8_t *tx_data, uint16_t size,GPIO_TypeDef* CS_GPIO_PORT, uint16_t CS_GPIO_Pin); //Taken from mcuFunctions.h
void spiTransmitReceive(SPI_HandleTypeDef *hspi,uint8_t *tx_data, uint8_t *rx_data ,uint16_t size,GPIO_TypeDef* CS_GPIO_PORT, uint16_t CS_GPIO_Pin); //Taken from mcuFunctions.h
void spiReceive(SPI_HandleTypeDef *hspi,uint8_t *rx_data, uint16_t size,GPIO_TypeDef* CS_GPIO_PORT, uint16_t CS_GPIO_Pin); //Taken from mcuFunctions.h

#endif /*__PAL_H */

/** @}*/