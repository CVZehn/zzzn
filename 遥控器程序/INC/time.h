#ifndef _TIME_H_
#define _TIME_H_
#include "reg52.h"
//-----------------------------------------------

#define FOSC 11059200L

#define T1MS (65536-FOSC/1)      //1Tģʽ

sfr AUXR = 0x8e;                    //Auxiliary register

typedef unsigned char BYTE;
typedef unsigned int WORD;



#define pole_up 1
#define pole_down 2
#define pole_stop 3

#define pedestrian 1//����
#define vehicle    2//����
#define all        3//���˺ͳ���

sbit LED1=P1^0;
sbit LED2=P1^1;

#endif

