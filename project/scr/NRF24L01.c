#include <reg52.h>
#include "NRF24L01.h"
//****************************************NRF24L01端口定义***************************************
sbit CE   = P1^7;
sbit CSN  = P1^6;
sbit SCK  = P1^5;
sbit MOSI = P1^4;//修改后
sbit MISO = P1^3;

//*********************************************NRF24L01*************************************
uchar const TX_ADDRESS[5]={0x00,0x00,0x00,0x00,0xA1};//接收本机 地址
uchar const RX_ADDRESS[5]={0x00,0x00,0x00,0x00,0xA1};//发送目标 地址
//***************************************NRF24L01寄存器指令*******************************************************
#define ADR_WIDTH       5   	// 3-5  字节 地址设定长度
#define PLOAD_WIDTH     4    	// 1-32 字节 数据设定长度
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//******************************************************************************************
uchar 	bdata sta;   //状态标志
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
//*****************************************长延时*****************************************
void delays(unsigned char s)
{
	unsigned char i;
	for(i=0; i<s; i++)
 {
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
 }
}	
/****************************************************************************************************
/*功能：NRF24L01的 SPI同步读写时序
/****************************************************************************************************/
uchar SPI_RW(uchar reg)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (reg & 0x80);         // output 'uchar', MSB to MOSI
		reg = (reg << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		reg |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(reg);           		  // return read uchar
}
/****************************************************************************************************
/*功能：NRF24L01的SPI读寄存器时序
/****************************************************************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	return(reg_val);        // return register value
}
/****************************************************************************************************/
/*功能：NRF24L01读写寄存器函数
/****************************************************************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	return(status);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
/*功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
/****************************************************************************************************/
void SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar i;
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	SPI_RW(reg);       		// Select register to write to and read status uchar
	for(i=0;i<uchars;i++)
	pBuf[i] = SPI_RW(0);    // 
	CSN = 1;                           
}
/*********************************************************************************************************
/*功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
/*********************************************************************************************************/
void SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,i;
	CSN = 0;            //SPI使能       
	status = SPI_RW(reg);   
	for(i=0; i<uchars;i++) //
		SPI_RW(*pBuf++);
	CSN = 1;           //关闭SPI
}
/***********************************************************************************************************
/*功能：发送 TX_buf中数据
/**********************************************************************************************************/
void nRF24L01_TX_Mode(unsigned char * TX_buf)
{
	CE=0;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, TX_buf, PLOAD_WIDTH);// 装载数据	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);// IRQ收发完成中断响应，16位CRC，发送
	CE=1;		 //置高CE，激发数据发送
	delays(250);//模块发送延时重要120-250之间
//	Delay(250);//模块发送延时重要120-250之间
}
/****************************************************************************************************/
/*功能：数据接收模式 
/****************************************************************************************************/
void nRF24L01_RX_Mode(unsigned char * RX_buf)
{
	CE=0;
    SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);    // 写本地地址	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);// IRQ收发完成中断响应，16位CRC，接收
	sta=SPI_Read(STATUS);	// 读取状态寄存其来判断数据接收状况
	if(RX_DR)				// 判断是否接收到数据
	SPI_Read_Buf(RD_RX_PLOAD,RX_buf,PLOAD_WIDTH);	//读取数据完成标志
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	CE = 1;
    delays(200);
	//Delay(200);Delay(200);
}
/****************************************************************************************
  NRF24L01初始化
//***************************************************************************************/
void init_NRF24L01(void)
{

	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低
	delays(200);delays(200);delays(200);
 	CE=0;    // chip enable
 	CSN=1;   // Spi  disable 
 	SCK=0;   // 
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS,ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS,ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, PLOAD_WIDTH); //设置接收数据长度，
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB
    delays(200);delays(200);
}

