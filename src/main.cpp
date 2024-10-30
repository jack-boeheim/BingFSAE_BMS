/*******************************************************************************
Copyright (c) 2020 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensor.
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "adbms_main.h"
#include "mbed.h"


/*
https://os.mbed.com/platforms/SDP_K1/
Mbed SDP-K1 board MOSI, MOSI, SCK definition.
Arduino Header Pinout
MOSI = D11
MISO = D12
SCK  = D13
CSB =  D10
*/
int main(void);
void spi_init(void);
void set_fan_pwm(float PWM_speed, int PWM_period_ns);
void test_fan(void);

DigitalOut chip_select(D10);                /* SPI chip select */
DigitalOut mosi(D11);                       /* MOSI */
DigitalIn miso(D12);                        /* MISO */
DigitalOut sclk(D13);                       /* SCK */
SPI spi(D11, D12, D13);                     /* SPI MOSI, MISO, SCK */
// Serial pc(USBTX, USBRX);                    /* USB Tx,Rx */
Timer timer;

PwmOut fan(PE_8); //nucleo f pwm pin 
DigitalIn fan_status(A1); //stealing this pin for the fan state input

int main()
{
    float fan_speed = .50; 
    int pwm_period_us = 40;
  //  spi_init();
  //  adbms_main();

    set_fan_pwm(fan_speed, pwm_period_us);

    while(1);

    return 0;
}

void set_fan_pwm(float PWM_speed, int PWM_period_ns){
    fan.write(PWM_speed); //percentage of 100 i.e. .25, .50, etc
    fan.period_us(PWM_period_ns); //tested at 40 us (25 kHz)
}

void test_fan(void){
    float fan_speed = .50; 
    int pwm_period_us = 40;

    set_fan_pwm(fan_speed, pwm_period_us);
    wait_us(3000000); //wait 3 seconds for fan to turn everything on and signal to reach board 

    if (fan_status.read() == 1) { //gotta figure out if high is good or bad 
        printf("bad!\n\n");
    }
    else if (fan_status.read() == 0) { //see line 64 
        printf("good!\n\n");
    }
    else{
        printf("how tf did we get here \n\n");
    }
 
}


void spi_init()
{   
    chip_select = 1;
    spi.format(8,0);            /* 8bit data, CPOL-CPHA= 0*/
    spi.frequency(2000000);     /* SPI clock 2Mhz */
    // pc.baud(115200);              /* Usb baud rate */   
}