#include "project_config.h"

//����������ʽ��ת�� A-B- --> B- --> B-A --> A --> AB --> B
// --> BA- --> A- --> A-B-
uchar upstep8_table[]={0x05,0x01,0x09,0x08,0x0A,0x02,0x06,0x04};
 
//����������ʽ��ת�� A --> B --> A- --> B-
uchar upstep4_table[]={0x08,0x02,0x06,0x01};
 
 
//******************************
//		�������ֹͣ����
//���ã�ֹͣ
//******************************
void MotorStop(void)
{
	P2 = 0x01;
}
 
 
//******************************
//		���������������
//���ã�ͨ������var���Ƶ綯����ת�ٸߵͣ�ͨ������state�жϵ綯��������ת
//			state��0 ��ת��state: 1 ��ת
//ʹ��8���ܹ�ʵ�ֱȽ�ƽ����ת����ʹ��4��ʱ����𶯱Ƚϴ�
//******************************
void MotorSpeedOrDirection(uint var, uchar state)
{
	uchar i=0;
	if(!state)
	{
		for(i=0; i<8; i++)
		{
			P2=upstep8_table[i]<<1;
			delay(var);
		}
	}else{
		for(i=7; i>0; --i)
		{
			P2=upstep8_table[i]<<1;
			delay(var);
		}
	}
}
 
 
//******************************
//		�������ƺ���
//���ã�ͨ������var������תȦ��
//******************************
void ConStep(uchar var)
{
	uchar i=0,j=0,m=0,k=0;
	while(k++<var)
	{
		for(i=0;i<20;i++)
		{
			for(m=0;m<20;m++)
			{
				P2=upstep8_table[j]<<1;
				j++;
				if(j>=8)             //ÿ8��һ��ѭ��
					j=0;
				delay(1);
			}
		}
	}
}

void move(uchar i)
{
	P2=upstep8_table[i]<<1;
}

