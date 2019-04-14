#ifndef _LDC1314_H_
#define _LDC1314_H_

#define LDC1314_ADDR_READ  0x55	 //设备7位地址为0x2A 最低位1
#define LDC1314_ADDR_WRITE 0x54  //设备7位地址为0x2A 最低位0

// LDC COMMANDS
#define LDC13xx16xx_CMD_DATA_MSB_CH0	0x00
#define LDC13xx16xx_CMD_DATA_LSB_CH0	0x01
#define LDC13xx16xx_CMD_DATA_MSB_CH1	0x02
#define LDC13xx16xx_CMD_DATA_LSB_CH1	0x03
#define LDC13xx16xx_CMD_DATA_MSB_CH2	0x04
#define LDC13xx16xx_CMD_DATA_LSB_CH2	0x05
#define LDC13xx16xx_CMD_DATA_MSB_CH3	0x06
#define LDC13xx16xx_CMD_DATA_LSB_CH3	0x07
#define LDC13xx16xx_CMD_REF_COUNT_CH0	0x08
#define LDC13xx16xx_CMD_REF_COUNT_CH1	0x09
#define LDC13xx16xx_CMD_REF_COUNT_CH2	0x0A
#define LDC13xx16xx_CMD_REF_COUNT_CH3	0x0B
#define LDC13xx16xx_CMD_OFFSET_CH0	0x0C
#define LDC13xx16xx_CMD_OFFSET_CH1	0x0D
#define LDC13xx16xx_CMD_OFFSET_CH2	0x0E
#define LDC13xx16xx_CMD_OFFSET_CH3	0x0F
#define LDC13xx16xx_CMD_SETTLE_COUNT_CH0	0x10
#define LDC13xx16xx_CMD_SETTLE_COUNT_CH1	0x11
#define LDC13xx16xx_CMD_SETTLE_COUNT_CH2	0x12
#define LDC13xx16xx_CMD_SETTLE_COUNT_CH3	0x13
#define LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH0 	0x14
#define LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH1 	0x15
#define LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH2 	0x16
#define LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH3 	0x17
#define LDC13xx16xx_CMD_STATUS 	0x18
#define LDC13xx16xx_CMD_ERROR_CONFIG 	0x19
#define LDC13xx16xx_CMD_CONFIG 	0x1A
#define LDC13xx16xx_CMD_MUX_CONFIG 	0x1B
#define LDC13xx16xx_CMD_RESET_DEVICE 	0x1C
#define LDC13xx16xx_CMD_SYSTEM_CLOCK_CONFIG	0x1D
#define LDC13xx16xx_CMD_DRIVE_CURRENT_CH0	0x1E
#define LDC13xx16xx_CMD_DRIVE_CURRENT_CH1 	0x1F
#define LDC13xx16xx_CMD_DRIVE_CURRENT_CH2 	0x20
#define LDC13xx16xx_CMD_DRIVE_CURRENT_CH3	0x21
#define LDC13xx16xx_CMD_MANUFACTID	0x7E
#define LDC13xx16xx_CMD_DEVID	0x7F

#define uchar unsigned char 
#define uint  unsigned int  
#define ulong unsigned long 
    

#define u8 unsigned char 
#define u16  unsigned int  

u16 LDC_read_CHx(u8 CHx);	//读Channel x Conversion Result
void LDC1314_Init(void)	;//初始化LDC1314和I2C

#endif
