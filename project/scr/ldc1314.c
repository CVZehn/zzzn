#include <reg52.h>
#include "intrins.h"
#include "ldc1314.h"
#include "timer0.h"
#include "oled.h" 

sbit IIC_SCL=P0^1;
sbit IIC_SDA=P0^2;
sbit XCLK=P0^3;

void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	i = 50;
	while (--i);
}


//初始化IIC
void IIC_Init(void)
{					     
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay5us();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay5us();
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	Delay5us();
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay5us();
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	Delay5us();							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u16 ucErrTime=0;//u8 ucErrTime=0;
	IIC_SDA=1;Delay5us();//delay_us(1);	   
	IIC_SCL=1;Delay5us();//delay_us(1);	 
	while(IIC_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)//if(ucErrTime>250)
		{
			IIC_Stop();
           // break;
			return 1;
		}
	}
    Delay5us();Delay5us();
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	Delay5us();//delay_us(2);
	IIC_SDA=0;
	Delay5us();//delay_us(2);
	IIC_SCL=1;
	Delay5us();//delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_SDA=1;
	Delay5us();//delay_us(2);
	IIC_SCL=1;
	Delay5us();//delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay5us();//delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		Delay5us();//delay_us(2); 
		IIC_SCL=0;	
		Delay5us();//delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA=0;
        Delay5us();//delay_us(2);
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay5us();//delay_us(2);
        receive<<=1;
        IIC_SCL=1; 
        Delay5us();//delay_us(2);
        if(IIC_SDA)receive++;   
    }				
    if (ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

void LCD_write_16bit(u8 regaddr,u16 dat)	//写data到寄存器，regaddr为寄存器的地址
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//发送设备地址，写
	Ack1 = IIC_Wait_Ack();
	
	IIC_Send_Byte(regaddr);	//寄存器地址
	Ack2 = IIC_Wait_Ack();
	
	IIC_Send_Byte(dat>>8);	//发送高八位
	Ack3 = IIC_Wait_Ack();
	
	IIC_Send_Byte(dat&0xff);	//发送低八位
	Ack4 = IIC_Wait_Ack();
	
	IIC_Stop();//产生一个停止条件


    OLED_ShowNum(50,2,Ack1,1,16);
    OLED_ShowNum(60,2,Ack2,1,16);
    OLED_ShowNum(70,2,Ack3,1,16);
    OLED_ShowNum(80,2,Ack4,1,16);
}



u16 LCD_read_16bit(u8 regaddr)	//读寄存器regaddr的值，regaddr为寄存器的地址
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//发送设备地址，写
	Ack1 = IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);	//寄存器地址
	Ack2 = IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_READ);//发送设备地址，读
	Ack3 = IIC_Wait_Ack();
	data_temp1 = IIC_Read_Byte(0);//IIC读取高位字节
	data_temp = IIC_Read_Byte(0);//IIC读取低位字节
	IIC_Stop();//产生一个停止条件
	
    OLED_ShowNum(50,0,Ack1,1,16);
    OLED_ShowNum(60,0,Ack2,1,16);
    OLED_ShowNum(70,0,Ack3,1,16);
    OLED_ShowNum(80,0,Ack4,1,16);
	//printf (" Ack1 %d\r\n Ack2 %d\r\n Ack3 %d, \r\n",Ack1,Ack2,Ack3);
	//printf ("data_temp1 %d,data_temp %d \r\n",data_temp1,data_temp);
	
	data_temp = data_temp + (data_temp1<<8) ;	//数据合并
	
	//printf("%d \r\n",data_temp); //串口发送看结果无误
	return data_temp;



}



u16 LDC_read_CHx(u8 CHx)	//读Channel x Conversion Result
{
	u8 reg_addr1 = 0;	//MSB地址
	u8 reg_addr = 0;	//LSB地址
	u16 dat = 0;			//接收到的数据
	
	switch(CHx)
	{
		case 0:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH0;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH0;	//不知道LSB寄存器有什么用，手册也没见有
		}break;
		case 1:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH1;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH1;
		}break;
		case 2:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH2;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH2;
		}break;
		case 3:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH3;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH3;
		}break;
		default: return 0;	//输入错误
		
	}
	//不知道LSB寄存器有什么用，手册也没见有
	dat = LCD_read_16bit(reg_addr);	//读取寄存器值
//	dat = dat & 0x0FFF; 	//转换结果在 [11:0]，把它取出来
	
	return dat;
		


}

void LDC1314_Init(void)	//初始化LDC1314和I2C
{
	IIC_Init();							//IIC初始化
//	LDC_ADDR = 0;	//LDC的7bit地址为0X2A，注意是7bit 即 0101 010_  最低位为读写位
//	LDC_SD = 0;		//退出Shutdown Mode
	delay(500);	//延时500ms
	
	
	LCD_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH0,0X04D6);
	
	LCD_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH1,0X04D6);
	LCD_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH2,0X04D6);
	LCD_write_16bit(LDC13xx16xx_CMD_REF_COUNT_CH3,0X04D6);
	
	LCD_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH0,0X000A);
	LCD_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH1,0X000A);
	LCD_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH2,0X000A);
	LCD_write_16bit(LDC13xx16xx_CMD_SETTLE_COUNT_CH3,0X000A);
	
	LCD_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH0,0X1002);
	LCD_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH1,0X1002);
	LCD_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH2,0X1002);
	LCD_write_16bit(LDC13xx16xx_CMD_CLOCK_DIVIDERS_CH3,0X1002);
	
	LCD_write_16bit(LDC13xx16xx_CMD_ERROR_CONFIG,0X0000);
	
	LCD_write_16bit(LDC13xx16xx_CMD_MUX_CONFIG,0x020c);
	
	LCD_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH0,0x9800);
	LCD_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH1,0x9800);
	LCD_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH2,0x9800);
	LCD_write_16bit(LDC13xx16xx_CMD_DRIVE_CURRENT_CH3,0x9800);

//	LCD_write_16bit(LDC13xx16xx_CMD_CONFIG,0x0104);		//bit13 清零 退出睡眠模式
	

	LCD_write_16bit(LDC13xx16xx_CMD_RESET_DEVICE,0x0600);
	LCD_write_16bit(LDC13xx16xx_CMD_SYSTEM_CLOCK_CONFIG,0x0200);
	LCD_write_16bit(LDC13xx16xx_CMD_CONFIG,0x1601);

}



























    