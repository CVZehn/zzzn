#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#include <reg52.h>
#include "intrins.h"
#include "motor.h"
#include "timer0.h"
#include "oled.h" 
#include "usart1.h" 
#include "NRF24L01.h"
#include "GP2Y_ADC.h"
#include "ldc1314.h"
#include "progress.h"



#define uchar unsigned char
#define uint unsigned int
#define command1 11
#define command2 22
#define com_new 0
#define com_dead 4000

#define pole_up 1
#define pole_down 2
#define pole_stop 3

#define pedestrian 1//����
#define vehicle    2//����
#define all        3//���˺ͳ���

#define keep_time        4000//5s

#define Pedestrian_distance 200//����

#define auto_model 0
#define cont_model 1

extern uchar Semaphore;//�ź��� ������ͽ�������־
extern uchar comm_pole;//�ź��� ������ͽ�������־
               

//0����    1�������Ӧ���� 2����⵽��������


extern  u8 RXBUFF[4],RXBUFF_L[4];//RXBUFF_L[4]:��һ���յ���ָ��ֵ
extern  u8 TXBUFF[4];
extern  uint comlive_time;
extern  uint rise_time;
extern uchar step;
extern uchar model;
extern uchar yuyin_last ;
extern uint yuyintime  ;
    
sfr P4   = 0xC0;

sbit KEY1=P3^3;
sbit KEY2=P3^4;
sbit Metal=P0^2;
sbit SW2 =P3^7;
sbit SW1 =P4^1;
sbit ledg =P4^2;
sbit ledr =P4^3;
sbit yuyin0 =P0^4;
sbit yuyin1 =P0^3;

#endif
