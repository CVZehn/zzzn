#ifndef _TIMER0_H_
#define _TIMER0_H_

#include <reg52.h>
#include "intrins.h"


    
static unsigned int count;
static unsigned int endcount;

void delay(uint ims);               //变量延时函数
void timer0_Init();				//中断0初始化
void Delay500ms()	;	//@11.0592MHz

#endif