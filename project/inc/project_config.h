#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#include <reg52.h>
#include "intrins.h"
#include "motor.h"
#include "timer0.h"
#include "oled.h" 
#include "usart1.h" 
#include "usart2.h" 
#include "NRF24L01.h"
#include "GP2Y_ADC.h"
#include "ldc1314.h"
#include "progress.h"



#define uchar unsigned char
#define uint unsigned int
#define open    101
#define close   102
#define stop    103
#define cont    104
#define nopass    105
#define command1 11
#define command2 22
#define com_new 0
#define com_dead 5000

#define pole_up 1
#define pole_down 2
#define pole_stop 3


extern uchar Semaphore;//信号量 ：红外和金属检测标志
extern uchar comm_pole;//信号量 ：红外和金属检测标志
               

//0：无    1：红外感应到人 2：检测到金属物体


extern  u8 RXBUFF[4],RXBUFF_L[4];//RXBUFF_L[4]:上一条收到的指令值
extern  u8 TXBUFF[4];
extern  u8 comlive_time;
extern uchar step;
    
sfr P4   = 0xC0;

sbit KEY1=P3^3;
sbit KEY2=P3^4;
sbit Metal=P3^6;
sbit SW1 =P4^1;
sbit SW2 =P4^2;

#endif
