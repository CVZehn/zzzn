#include "reg52.h"
#include "NRF24L01.h"
#include "time.h"

sbit KEY3=P3^7;
sbit KEY4=P3^6;
sbit KEY5=P3^3;
sbit KEY6=P3^2;
//行人：蓝灯亮 车辆：红灯亮 

void Delay150us();	//@11.0592MHz
void Delay50ms();

unsigned char TXBUFF[]={112,112,112,112};
unsigned char RXBUFF[4];

void main()
{
    KEY3=1;
    KEY4=1;
    KEY5=1;
    KEY6=1;
    timer_init();
    Delay50ms();
    Delay50ms();
    Delay50ms();
    init_NRF24L01();
    Delay50ms();
    while(1)
    {
        
        TXBUFF[0]=0;TXBUFF[1]=0;TXBUFF[2]=0;TXBUFF[3]=0;
        
//        
        
        if(KEY3==0)//禁止通过 杆升起
        {
                Delay50ms();
            if(KEY3==0)
            {
                TXBUFF[1]=pole_up;
                while(KEY3==0);
            }
            
        }
        else if(KEY4==0)//允许通过 杆下降
        {
                Delay50ms();
             if(KEY4==0)
            {
                TXBUFF[1]=pole_down;
                while(KEY4==0);
            }
        }
        else if(KEY5==0)//杆停止运动
        {
                Delay50ms();
            if(KEY5==0)
            {
                TXBUFF[1]=pole_stop;
                while(KEY5==0);
            }
        }
        else if(KEY6==0)
        {
            Delay50ms();
            if(KEY6==0)
            {
                TXBUFF[1]=9;
                while(KEY6==0);
            }
        }
        else
        {
            TXBUFF[0]=0;TXBUFF[1]=0;TXBUFF[2]=0;TXBUFF[3]=0;
        }
        
        nRF24L01_TX_Mode(TXBUFF);Delay150us();
        nRF24L01_RX_Mode(RXBUFF);Delay150us();
        if(RXBUFF[1]==pedestrian)//
        {
            LED2=0;
            LED1=1;
        }
        else if(RXBUFF[1]==vehicle)//
        {
            LED1=0;
            LED2=1;
        }
        else if(RXBUFF[1]==all)//
        {
            LED2=0;
            LED1=0;
        }
        else
        {
            LED2=1;
            LED1=1;
        }
//        
    }
}

void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay150us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 2;
	j = 153;
	do
	{
		while (--j);
	} while (--i);
}
