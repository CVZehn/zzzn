#include "project_config.h"

void (*run)();
uchar step;
//******************************
//		��ʱ��0��ʼ������
//���ã���ʼ����ʱ��0
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
//		�жϴ�����
//���ã���ʱ��0���жϴ���
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
//��ʱi ms
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
