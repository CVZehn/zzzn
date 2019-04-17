����void InitLDC1314��void��

����{

����I2C_Init����;

����while��WordRead1314��DEVICE_ID����=0x3025��;//�ȴ���I2C����

����//WordWrite1314��RESET_DEV��0x8800��;

����/*����LDC1314*/

����WordWrite1314��CONFIG��0x2451��;

����/*Ĭ��ֵ��˯��ģʽ������������*/

����WordWrite1314��CLOCK_DIVIDERS_CH0��0X500a��;//fSENSOR����Ƶ��fCLK 2��Ƶ

����/* ʱ��Դ��Ƶѡ�񣬷�Ƶϵ��1��1

����Using L and C�� ��fSENSOR = 1/2��&TImes;sqrt��L &TImes;C��

����ע��fSENSOR��С��8.75 MHz��fSENSOR��Ƶϵ��������1

����ע��fREF0�����ٴ��� 4 �� fSENSOR���Ծ���fREF0��Ƶϵ��

����fin0 = fSENSOR0/CH0_FIN_DIVIDER

����fREF0 = fCLK / CH0_FREF_DIVIDER

����*

����WordWrite1314��SETTLECOUNT_CH0��0x0A60��;

����/* ������Settling��ʱ������

���������Q=Rp*sqrt��C/L��

�����Ĵ���ֵС��10ȡ10������10ȡ��ֵ����Ӧ��������������

����CHx_SETTLECOUNT �� QSENSORx �� fREFx / ��16 �� fSENSORx�� ��10���ƣ�

������1��fSENSORx:xͨ��������Ƶ��

������2��fREFx:xͨ���ο�Ƶ��

������3��QSENSORx��Ʒ��������Q=Rp ��sqrt��C/L��

����ע�⣬����ʽ�ұ������С��������ȡ��

��������ʱ��tSx = ��CH0_SETTLECOUNT*16�� / fREF0

����ͨ���л�ʱ��tCsd = Channel Switch Delay = 692 ns + 5 / fref ��us��

����*/

����WordWrite1314��RCOUNT_CH0��0xFFFF��;

����/*ת��ʱ������

������ת��ʱ��tCx = TSAMPLE������ʱ�䣩 �C tSx �C tCsd ��us��

��������ʽ����Ĵ�����ֵ����С��������ȡ������ȥ������

����ת��ʱ��tCx = ��CHx_RCOUNT �� 16�� / fREFx ��10���ƣ�

����ע��Ĵ�����ֵ��Ϊ16����

����*/

����/*ʹ��ERROR_CONFIGĬ��ֵ�������жϹر�*/

����WordWrite1314��DRIVE_CURRENT_CH0��0x9800��;

����/*�����������������ã�ʹ����ͬʱע��CONFIG�Ĵ����������

�����������ֲ��Figure 15����IDRIVEֵ��10���ƣ���INIT_DRIVEֵ��ʹ�ã�����Ĭ��

����ʹ��CH0_IDRIVE��15:11��ʱ��Rp Override���

����ʹ��CH0_INIT_IDRIVE��10:6��ʱ����֤AUTO_AMP_DISλ����λ

����*/

����//WordWrite1314��MUX_CONFIG��0x801��;

����/* ��ͨ��ת��ѡ�������������˲�������ѡ��ʹ����ͬʱע��CONFIG�Ĵ���������ã�

������a����AUTOSCAN_ENλΪb1ʹ������ģʽ��sequential mode��

������b����RR_SEQUENCEλΪb00ʹ������ͨ��������ת����channel 0��channel 1��

������c����DEGLITCHλΪb100�趨�����������˲���������ֵ�������г��Ƶ��fTank��

����*/

����//WordWrite1314��OFFSET_CH0��0x00��;

����/*Ƶ��ƫ�ƣ�����������

�����������ô�ÿ������ֵ�м�ȥƫ��ֵ���Բ���Ƶ��ƫ�ƻ���󻯶�̬��Χ����������

����fOFFSET0 = CH0_OFFSET * ��fREF0/2^16��

������ƫ��ֵӦС�� fSENSORx_MIN / fREFx

����*/

����//WordWrite1314��RESET_DEV��0x00��;

����/*����������ã���10:9�ݣ�������ͨ����Ч

�������ڴ������źű仯��С��������25����ϵͳ�����������������ṩ���ߵķֱ��ʣ����ȣ�

����*/

����/*

����״̬�Ĵ�����STATUS��ERROR_CONFIG

����*/

����WordWrite1314��CONFIG��0x1501��;

����/*Ĭ������ת��������ͨ��0��˯��ģʽ�أ�Rp Override��

�����͹���ģʽ�أ��������Զ�����У����AUTO_AMP���أ�Refʱ��Դѡ�ⲿʱ��Դ

����INTB�жϿ����ߵ���������*/

����}

����/*��ȡLDC1314ת��������������Ϊѡ��ͨ����*/

����uint LDC1314_Result��uchar Channal��

����{

����uint temp=0;

����switch��Channal��

����{

����case 0x00��

����temp=WordRead1314��DATA_CH0��; //ͨ��0

����break;

����case 0x01��

����temp=WordRead1314��DATA_CH1��; //ͨ��1

����break;

����case 0x02��

����temp=WordRead1314��DATA_CH2��; //ͨ��2

����break;

����case 0x03��

����temp=WordRead1314��DATA_CH3��; //ͨ��3

����break;

����default ��

����break;

����}

����while����temp&0xF000����=0��;

����/*�д���Flag�������������ʱע��˾䣬���ܳ���

����0x000 = under range��0xfff = over range

����*/

����return temp;

����}

����LDC1314.H�ļ�

����#ifndef __LDC1314_H

����#define __LDC1314_H

����#include ��msp430f169.h��

����#include ��config.h��

����static void I2C_Init����;

����static void I2C_Start����;

����static void I2C_Stop����;

����static void I2C_SendACK��uchar ack��;

����static uchar I2C_RecvACK����;

����static uchar I2C_SendByte��uchar dat��;

����static uchar I2C_RecvACK����;

����static void WordWrite1314��uchar REG_Address��uint REG_data��;

����static uint WordRead1314��uchar REG_Address��;

����void InitLDC1314��void��;

����uint LDC1314_Result��uchar Channal��;

����//****����LDC1314Ӳ���ӿڣ���ͬӲ���޸Ĵ˴����ɣ�***********

����#define LDC1314DIR P1DIR

����#define LDC1314OUT P1OUT

����#define LDC1314IN P1IN

����#define LDC_SCL BIT2

����#define LDC_SDA BIT3

����#define LDC_INTB BIT4//�ж�

����#define LDC_SD BIT5//�øߣ���������ģʽ

����//*********I2CӲ����ؽӿں���**************

����#define LDC_SCL_OUT���� LDC1314DIR |= LDC_SCL // SCL�����

����#define LDC_SCL_H���� LDC1314OUT |= LDC_SCL // SCL����

����#define LDC_SCL_L���� LDC1314OUT &= ~LDC_SCL // SCL����

����#define LDC_SDA_OUT���� LDC1314DIR |= LDC_SDA // SDA�����

����#define LDC_SDA_H���� LDC1314OUT |= LDC_SDA // SDA����

����#define LDC_SDA_L���� LDC1314OUT &= ~LDC_SDA // SDA����

����#define LDC_SDA_IN���� LDC1314DIR &= ~LDC_SDA // SDA������

����#define LDC_SDA_DAT���� ��LDC1314IN&LDC_SDA�� // SDA��������

����#define SlaveAddress 0x2A //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ��ADDR�Ӹߵ�ƽ����ַ��Ϊ0x2B

����#define DATA_CH0 0x00 //Channel 0 Conversion Result and Error Status

����#define DATA_CH1 0x02

����#define DATA_CH2 0x04

����#define DATA_CH3 0x06

����#define RCOUNT_CH0 0x08 //Reference Count setting for Channel 0

����#define RCOUNT_CH1 0x09

����#define RCOUNT_CH2 0x0A

����#define RCOUNT_CH3 0x0B

����#define OFFSET_CH0 0x0C //Offset value for Channel 0

����#define OFFSET_CH1 0x0D

����#define OFFSET_CH2 0x0E

����#define OFFSET_CH3 0x0F

����#define SETTLECOUNT_CH0 0x10 //Channel 0 Settling Reference Count

����#define SETTLECOUNT_CH1 0x11

����#define SETTLECOUNT_CH2 0x12

����#define SETTLECOUNT_CH3 0x13

����#define CLOCK_DIVIDERS_CH0 0x14

����#define CLOCK_DIVIDERS_CH1 0x15

����#define CLOCK_DIVIDERS_CH2 0x16

����#define CLOCK_DIVIDERS_CH3 0x17

����#define STATUS 0x18 //Device Status Report

����#define ERROR_CONFIG 0x19 //Error Reporting Configuration

����#define MUX_CONFIG 0x1A //Channel Multiplexing Configuration

����#define CONFIG 0x1B //Conversion Configuration

����#define RESET_DEV 0x1C //Reset Device

����#define DRIVE_CURRENT_CH0 0x1E //Channel 0 sensor current drive configuration

����#define DRIVE_CURRENT_CH1 0x1F

����#define DRIVE_CURRENT_CH2 0x20

����#define DRIVE_CURRENT_CH3 0x21

����#define MANUFACTURER_ID 0x7E //����ID��Ĭ��ֵ0x5449

����#define DEVICE_ID 0x7F //�豸ID��Ĭ��ֵ0x3054

����#endif