#include "project_config.h"

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;

sbit RST=P1^1;


u8 RXBUFF[4],RXBUFF_L[4];
u8 TXBUFF[4]={11,11,11,11};


void Delay150us();	//@11.0592MHz

void main()
{
    
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    
    SW1=1;
    SW2=1;
    Metal = 1;
    
    run=MotorStop;//初始化指针函数防止野指针的出现
    timer0_Init();
    InitADC();
    init_NRF24L01();
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
    OLED_ShowString(10,0,"ready    ",16);
//        yuyin0=1;
//        yuyin1=1;
	while(1)
	{
        sence();
        nRF24L01_TX_Mode(TXBUFF);
        Delay150us();
        nRF24L01_RX_Mode(RXBUFF);
        Delay150us();
        remote_control();
	}
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