#ifndef _TIMER0_H_
#define _TIMER0_H_

#include <reg52.h>
#include "intrins.h"


    
static unsigned int count;
static unsigned int endcount;

void delay(uint ims);               //������ʱ����
void timer0_Init();				//�ж�0��ʼ��
void Delay500ms()	;	//@11.0592MHz
extern void (*run)();

#endif