　　void InitLDC1314（void）

　　{

　　I2C_Init（）;

　　while（WordRead1314（DEVICE_ID）！=0x3025）;//等待，I2C正常

　　//WordWrite1314（RESET_DEV，0x8800）;

　　/*重启LDC1314*/

　　WordWrite1314（CONFIG，0x2451）;

　　/*默认值，睡眠模式开，方便配置*/

　　WordWrite1314（CLOCK_DIVIDERS_CH0，0X500a）;//fSENSOR不分频，fCLK 2分频

　　/* 时钟源分频选择，分频系数1，1

　　Using L and C， 得fSENSOR = 1/2π&TImes;sqrt（L &TImes;C）

　　注意fSENSOR须小于8.75 MHz，fSENSOR分频系数才能置1

　　注意fREF0须至少大于 4 × fSENSOR，以决定fREF0分频系数

　　fin0 = fSENSOR0/CH0_FIN_DIVIDER

　　fREF0 = fCLK / CH0_FREF_DIVIDER

　　*

　　WordWrite1314（SETTLECOUNT_CH0，0x0A60）;

　　/* 建立（Settling）时间设置

　　计算得Q=Rp*sqrt（C/L）

　　寄存器值小于10取10，大于10取其值，且应满足以下条件：

　　CHx_SETTLECOUNT ≥ QSENSORx × fREFx / （16 × fSENSORx） （10进制）

　　（1）fSENSORx:x通道传感器频率

　　（2）fREFx:x通道参考频率

　　（3）QSENSORx：品质因数，Q=Rp ×sqrt（C/L）

　　注意，不等式右边算出有小数则向上取整

　　建立时间tSx = （CH0_SETTLECOUNT*16） / fREF0

　　通道切换时间tCsd = Channel Switch Delay = 692 ns + 5 / fref （us）

　　*/

　　WordWrite1314（RCOUNT_CH0，0xFFFF）;

　　/*转换时间设置

　　得转换时间tCx = TSAMPLE（采样时间） – tSx – tCsd （us）

　　由下式算出寄存器的值（有小数则向下取整（舍去））：

　　转换时间tCx = （CHx_RCOUNT × 16） / fREFx （10进制）

　　注意寄存器的值化为16进制

　　*/

　　/*使用ERROR_CONFIG默认值，错误中断关闭*/

　　WordWrite1314（DRIVE_CURRENT_CH0，0x9800）;

　　/*传感器驱动电流设置，使用请同时注意CONFIG寄存器相关配置

　　查数据手册的Figure 15，得IDRIVE值（10进制），INIT_DRIVE值不使用，保持默认

　　使用CH0_IDRIVE［15:11］时，Rp Override须打开

　　使用CH0_INIT_IDRIVE［10:6］时，保证AUTO_AMP_DIS位不置位

　　*/

　　//WordWrite1314（MUX_CONFIG，0x801）;

　　/* 多通道转换选择，输入尖峰脉冲滤波器带宽选择，使用请同时注意CONFIG寄存器相关配置！

　　（a）置AUTOSCAN_EN位为b1使能连续模式（sequential mode）

　　（b）置RR_SEQUENCE位为b00使能两个通道的数据转换（channel 0，channel 1）

　　（c）置DEGLITCH位为b100设定输入尖峰脉冲滤波器带宽，此值必须高于谐振频率fTank！

　　*/

　　//WordWrite1314（OFFSET_CH0，0x00）;

　　/*频率偏移（补偿）设置

　　可以设置从每个数据值中减去偏移值，以补偿频率偏移或最大化动态范围的样本数据

　　fOFFSET0 = CH0_OFFSET * （fREF0/2^16）

　　且偏移值应小于 fSENSORx_MIN / fREFx

　　*/

　　//WordWrite1314（RESET_DEV，0x00）;

　　/*输出增益设置，［10:9］，对所有通道有效

　　对于传感器信号变化量小于满量程25％的系统，设置输出增益可以提供更高的分辨率（精度）

　　*/

　　/*

　　状态寄存器：STATUS，ERROR_CONFIG

　　*/

　　WordWrite1314（CONFIG，0x1501）;

　　/*默认连续转换（单）通道0，睡眠模式关，Rp Override开

　　低功耗模式关，传感器自动幅度校正（AUTO_AMP）关，Ref时钟源选外部时钟源

　　INTB中断开，高电流驱动关*/

　　}

　　/*读取LDC1314转换结果，输入参数为选择通道数*/

　　uint LDC1314_Result（uchar Channal）

　　{

　　uint temp=0;

　　switch（Channal）

　　{

　　case 0x00：

　　temp=WordRead1314（DATA_CH0）; //通道0

　　break;

　　case 0x01：

　　temp=WordRead1314（DATA_CH1）; //通道1

　　break;

　　case 0x02：

　　temp=WordRead1314（DATA_CH2）; //通道2

　　break;

　　case 0x03：

　　temp=WordRead1314（DATA_CH3）; //通道3

　　break;

　　default ：

　　break;

　　}

　　while（（temp&0xF000）！=0）;

　　/*有错误Flag！，有输出增益时注意此句，可能出错！

　　0x000 = under range，0xfff = over range

　　*/

　　return temp;

　　}

　　LDC1314.H文件

　　#ifndef __LDC1314_H

　　#define __LDC1314_H

　　#include 《msp430f169.h》

　　#include “config.h”

　　static void I2C_Init（）;

　　static void I2C_Start（）;

　　static void I2C_Stop（）;

　　static void I2C_SendACK（uchar ack）;

　　static uchar I2C_RecvACK（）;

　　static uchar I2C_SendByte（uchar dat）;

　　static uchar I2C_RecvACK（）;

　　static void WordWrite1314（uchar REG_Address，uint REG_data）;

　　static uint WordRead1314（uchar REG_Address）;

　　void InitLDC1314（void）;

　　uint LDC1314_Result（uchar Channal）;

　　//****定义LDC1314硬件接口（不同硬件修改此处即可）***********

　　#define LDC1314DIR P1DIR

　　#define LDC1314OUT P1OUT

　　#define LDC1314IN P1IN

　　#define LDC_SCL BIT2

　　#define LDC_SDA BIT3

　　#define LDC_INTB BIT4//中断

　　#define LDC_SD BIT5//置高，则进入掉电模式

　　//*********I2C硬件相关接口函数**************

　　#define LDC_SCL_OUT（） LDC1314DIR |= LDC_SCL // SCL脚输出

　　#define LDC_SCL_H（） LDC1314OUT |= LDC_SCL // SCL拉高

　　#define LDC_SCL_L（） LDC1314OUT &= ~LDC_SCL // SCL拉低

　　#define LDC_SDA_OUT（） LDC1314DIR |= LDC_SDA // SDA脚输出

　　#define LDC_SDA_H（） LDC1314OUT |= LDC_SDA // SDA拉高

　　#define LDC_SDA_L（） LDC1314OUT &= ~LDC_SDA // SDA拉低

　　#define LDC_SDA_IN（） LDC1314DIR &= ~LDC_SDA // SDA脚输入

　　#define LDC_SDA_DAT（） （LDC1314IN&LDC_SDA） // SDA输入数据

　　#define SlaveAddress 0x2A //IIC写入时的地址字节数据，+1为读取，ADDR接高电平，地址则为0x2B

　　#define DATA_CH0 0x00 //Channel 0 Conversion Result and Error Status

　　#define DATA_CH1 0x02

　　#define DATA_CH2 0x04

　　#define DATA_CH3 0x06

　　#define RCOUNT_CH0 0x08 //Reference Count setting for Channel 0

　　#define RCOUNT_CH1 0x09

　　#define RCOUNT_CH2 0x0A

　　#define RCOUNT_CH3 0x0B

　　#define OFFSET_CH0 0x0C //Offset value for Channel 0

　　#define OFFSET_CH1 0x0D

　　#define OFFSET_CH2 0x0E

　　#define OFFSET_CH3 0x0F

　　#define SETTLECOUNT_CH0 0x10 //Channel 0 Settling Reference Count

　　#define SETTLECOUNT_CH1 0x11

　　#define SETTLECOUNT_CH2 0x12

　　#define SETTLECOUNT_CH3 0x13

　　#define CLOCK_DIVIDERS_CH0 0x14

　　#define CLOCK_DIVIDERS_CH1 0x15

　　#define CLOCK_DIVIDERS_CH2 0x16

　　#define CLOCK_DIVIDERS_CH3 0x17

　　#define STATUS 0x18 //Device Status Report

　　#define ERROR_CONFIG 0x19 //Error Reporting Configuration

　　#define MUX_CONFIG 0x1A //Channel Multiplexing Configuration

　　#define CONFIG 0x1B //Conversion Configuration

　　#define RESET_DEV 0x1C //Reset Device

　　#define DRIVE_CURRENT_CH0 0x1E //Channel 0 sensor current drive configuration

　　#define DRIVE_CURRENT_CH1 0x1F

　　#define DRIVE_CURRENT_CH2 0x20

　　#define DRIVE_CURRENT_CH3 0x21

　　#define MANUFACTURER_ID 0x7E //厂商ID，默认值0x5449

　　#define DEVICE_ID 0x7F //设备ID，默认值0x3054

　　#endif