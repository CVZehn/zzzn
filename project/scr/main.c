#include "project_config.h"

void Delay150us();	//@11.0592MHz

void main()
{
    u8 RXBUFF[4];
    u8 TXBUFF[4]={11,11,11,11};
    init_NRF24L01();
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
    OLED_ShowString(36,1,"34",16);//音量等级
	while(1)
	{
        nRF24L01_TX_Mode(TXBUFF);
        Delay150us();
        nRF24L01_RX_Mode(RXBUFF);
        if(RXBUFF[0]==101)
        {
            OLED_ShowString(36,3,"112",16);//音量等级
        }
        else if(RXBUFF[0]==102)
        {
            OLED_ShowString(36,3,"111",16);//音量等级
        }
        if(KEY1==0)
        {
            TXBUFF[0]=11;TXBUFF[1]=11;TXBUFF[2]=11;TXBUFF[3]=11;
        }
        else if(KEY2==0)
        {
            TXBUFF[0]=22;TXBUFF[1]=22;TXBUFF[2]=22;TXBUFF[3]=22;
        }
        Delay150us();
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