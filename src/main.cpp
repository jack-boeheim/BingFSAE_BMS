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
void set_fan_pwm(float &PWM_speed, int &PWM_period_us, int fan_select);
void test_fan(int fan_select);

DigitalOut chip_select(D10);                /* SPI chip select */
DigitalOut mosi(D11);                       /* MOSI */
DigitalIn miso(D12);                        /* MISO */
DigitalOut sclk(D13);                       /* SCK */
SPI spi(D11, D12, D13);                     /* SPI MOSI, MISO, SCK */
// Serial pc(USBTX, USBRX);                    /* USB Tx,Rx */
Timer timer;

PwmOut fan1(D2);  
PwmOut fan2(D3);  
PwmOut fan3(D4);  
PwmOut fan4(D5);  
PwmOut fan5(D6);  
PwmOut fan6(D7);  
PwmOut fan7(D8);  
PwmOut fan8(D9);  

DigitalIn fan_status(A1); //stealing this pin for the fan state input

int main()
{
    float fan_speed = .50; 
    int pwm_period_us = 40;


    pwm_period_us = 11627; //fan feedback testing, 86 hz
  //  spi_init();
  //  adbms_main();
    for (int i = 1; i<9; i++){
        set_fan_pwm(fan_speed, pwm_period_us, i);
    }

    while(1);

    return 0;
}

void set_fan_pwm(float &PWM_speed, int &PWM_period_us, int fan_select){
    //percentage of 100 i.e. .25, .50, etc for pwm_speed
    //tested at 40 us (25 kHz), for pwm_period_us

    switch (fan_select){
        case 1:
            fan1.write(PWM_speed);
            fan1.period_us(PWM_period_us);
            break;
        case 2:
            fan2.write(PWM_speed);
            fan2.period_us(PWM_period_us);
            break;
        case 3:
            fan3.write(PWM_speed);
            fan3.period_us(PWM_period_us);
            break;
        case 4:
            fan4.write(PWM_speed);
            fan4.period_us(PWM_period_us);
            break;
        case 5:
            fan5.write(PWM_speed);
            fan5.period_us(PWM_period_us);
            break;
        case 6:
            fan6.write(PWM_speed);
            fan6.period_us(PWM_period_us);
            break;
        case 7:
            fan7.write(PWM_speed);
            fan7.period_us(PWM_period_us);
            break;
        case 8:
            fan8.write(PWM_speed);
            fan8.period_us(PWM_period_us);
            break;
        default:
            printf("Non existant fan selected.");
    }

}

void test_fan(int fan_select){
    float fan_speed = .50; 
    int pwm_period_us = 40;

    set_fan_pwm(fan_speed, pwm_period_us, fan_select);
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