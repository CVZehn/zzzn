#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <reg52.h>
#include "intrins.h"


#define uchar unsigned char
#define uint unsigned int
 


void MotorSpeedOrDirection(uint var, uchar state);        //���Ƶ��ת�ٺͷ���
void ConStep(uchar var);			//����������
void MotorStop(void);    			//ֹͣ
void move(uchar i);

#endif