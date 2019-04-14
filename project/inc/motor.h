#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <reg52.h>
#include "intrins.h"


#define uchar unsigned char
#define uint unsigned int
 


void MotorSpeedOrDirection(uint var, uchar state);        //控制电机转速和方向
void ConStep(uchar var);			//步进控制器
void MotorStop(void);    			//停止

#endif