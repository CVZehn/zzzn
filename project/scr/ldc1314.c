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


//��ʼ��IIC
void IIC_Init(void)
{					     
	IIC_SCL=1;
	IIC_SDA=1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay5us();
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay5us();
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	Delay5us();
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay5us();
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	Delay5us();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_SDA=1;
	Delay5us();//delay_us(2);
	IIC_SCL=1;
	Delay5us();//delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay5us();//delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		Delay5us();//delay_us(2); 
		IIC_SCL=0;	
		Delay5us();//delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

void LCD_write_16bit(u8 regaddr,u16 dat)	//дdata���Ĵ�����regaddrΪ�Ĵ����ĵ�ַ
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//�����豸��ַ��д
	Ack1 = IIC_Wait_Ack();
	
	IIC_Send_Byte(regaddr);	//�Ĵ�����ַ
	Ack2 = IIC_Wait_Ack();
	
	IIC_Send_Byte(dat>>8);	//���͸߰�λ
	Ack3 = IIC_Wait_Ack();
	
	IIC_Send_Byte(dat&0xff);	//���͵Ͱ�λ
	Ack4 = IIC_Wait_Ack();
	
	IIC_Stop();//����һ��ֹͣ����


    OLED_ShowNum(50,2,Ack1,1,16);
    OLED_ShowNum(60,2,Ack2,1,16);
    OLED_ShowNum(70,2,Ack3,1,16);
    OLED_ShowNum(80,2,Ack4,1,16);
}



u16 LCD_read_16bit(u8 regaddr)	//���Ĵ���regaddr��ֵ��regaddrΪ�Ĵ����ĵ�ַ
{
	u16 data_temp = 0;
	u16 data_temp1 = 0;
	u8 Ack1,Ack2,Ack3,Ack4;
	Ack1=Ack2=Ack3=Ack4=2;
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_WRITE);	//�����豸��ַ��д
	Ack1 = IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);	//�Ĵ�����ַ
	Ack2 = IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(LDC1314_ADDR_READ);//�����豸��ַ����
	Ack3 = IIC_Wait_Ack();
	data_temp1 = IIC_Read_Byte(0);//IIC��ȡ��λ�ֽ�
	data_temp = IIC_Read_Byte(0);//IIC��ȡ��λ�ֽ�
	IIC_Stop();//����һ��ֹͣ����
	
    OLED_ShowNum(50,0,Ack1,1,16);
    OLED_ShowNum(60,0,Ack2,1,16);
    OLED_ShowNum(70,0,Ack3,1,16);
    OLED_ShowNum(80,0,Ack4,1,16);
	//printf (" Ack1 %d\r\n Ack2 %d\r\n Ack3 %d, \r\n",Ack1,Ack2,Ack3);
	//printf ("data_temp1 %d,data_temp %d \r\n",data_temp1,data_temp);
	
	data_temp = data_temp + (data_temp1<<8) ;	//���ݺϲ�
	
	//printf("%d \r\n",data_temp); //���ڷ��Ϳ��������
	return data_temp;



}



u16 LDC_read_CHx(u8 CHx)	//��Channel x Conversion Result
{
	u8 reg_addr1 = 0;	//MSB��ַ
	u8 reg_addr = 0;	//LSB��ַ
	u16 dat = 0;			//���յ�������
	
	switch(CHx)
	{
		case 0:
		{
			reg_addr1 = LDC13xx16xx_CMD_DATA_MSB_CH0;
			reg_addr 	= LDC13xx16xx_CMD_DATA_LSB_CH0;	//��֪��LSB�Ĵ�����ʲô�ã��ֲ�Ҳû����
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
		default: return 0;	//�������
		
	}
	//��֪��LSB�Ĵ�����ʲô�ã��ֲ�Ҳû����
	dat = LCD_read_16bit(reg_addr);	//��ȡ�Ĵ���ֵ
//	dat = dat & 0x0FFF; 	//ת������� [11:0]������ȡ����
	
	return dat;
		


}

void LDC1314_Init(void)	//��ʼ��LDC1314��I2C
{
	IIC_Init();							//IIC��ʼ��
//	LDC_ADDR = 0;	//LDC��7bit��ַΪ0X2A��ע����7bit �� 0101 010_  ���λΪ��дλ
//	LDC_SD = 0;		//�˳�Shutdown Mode
	delay(500);	//��ʱ500ms
	
	
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

//	LCD_write_16bit(LDC13xx16xx_CMD_CONFIG,0x0104);		//bit13 ���� �˳�˯��ģʽ
	

	LCD_write_16bit(LDC13xx16xx_CMD_RESET_DEVICE,0x0600);
	LCD_write_16bit(LDC13xx16xx_CMD_SYSTEM_CLOCK_CONFIG,0x0200);
	LCD_write_16bit(LDC13xx16xx_CMD_CONFIG,0x1601);

}



























    