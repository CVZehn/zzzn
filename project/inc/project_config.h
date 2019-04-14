#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#include <reg52.h>
#include "intrins.h"
#include "motor.h"
#include "timer0.h"
#include "oled.h" 
#include "NRF24L01.h"

#define uchar unsigned char
#define uint unsigned int
    

sbit KEY1=P3^3;
sbit KEY2=P3^4;

#endif
