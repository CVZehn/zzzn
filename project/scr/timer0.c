#include "project_config.h"

void (*run)();
uchar step;
//******************************
//		定时器0初始化函数
//作用：初始化定时器0
//******************************
void timer0_Init()
{
	EA = 1;
	TMOD = 0x11;
	ET0 = 1;
 
	TH0=(65536-1100)/256;
	TL0=(65536-1100)%256;
 
	TR0=1;
}
 
 
//******************************
//		中断处理函数
//作用：定时器0的中断处理
//******************************
void time0int(void) interrupt 1
{
	TH0=(65536-1100)/256;
	TL0=(65536-1100)%256;
    run();
	count++;
    if(Semaphore!=0)
    {
        comlive_time++;
    }
    if(comlive_time>com_dead)
    {
        comlive_time=com_new;
        Semaphore=0;
    }
}
 
 
//******************************
//
//延时i ms
//******************************
void delay(uint ims)
{
	endcount=ims;
	count=0;
	do{}while(count<endcount);
}
 
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
