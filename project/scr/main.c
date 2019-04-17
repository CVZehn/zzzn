#include "project_config.h"

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sbit RST=P1^1;


u8 RXBUFF[4],RXBUFF_L[4];
u8 TXBUFF[4]={11,11,11,11};


void Delay150us();	//@11.0592MHz

void main()
{
    P0M1 = 0;  
    P0M0 = 0;   //����Ϊ׼˫���
    
    run=Delay150us;//��ʼ��ָ�뺯����ֹҰָ��ĳ���
    timer0_Init();
    InitADC();
    usart2_init();
    init_NRF24L01();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
 //   OLED_ShowString(1,1,"34",16);
	while(1)
	{
        MotorSpeedOrDirection(1,0);
//        sence();
//        remote_control();
//        nRF24L01_TX_Mode(TXBUFF);
//        Delay150us();
//        nRF24L01_RX_Mode(RXBUFF);
//        Delay150us();
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