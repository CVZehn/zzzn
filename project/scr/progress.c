#include "project_config.h"

uchar Semaphore=0;
u8 comlive_time;

//Semaphore;//�ź��� ������ͽ�������־
                       //0����    1�������Ӧ���� 2����⵽��������
void null()
{
    
}    
void rr()
{
//   if(SW1==1)
//   {
       step--;
       move(step);
//   }
//   else
//   {
//       MotorStop();
//   }
}
void rl()
{
//   if(SW2==1)
//   {
       step++;
       move(step);
//   }
//   else
//   {
//       MotorStop();
//   }
}

void sence()
{
    Semaphore=0;
    if(Metal==1&&((GetADCResult(0)<<2)|ADC_LOW2)<100)
    {
        Semaphore=1;
        TXBUFF[1]=2;
    }
    if(Metal==0&&((GetADCResult(0)<<2)|ADC_LOW2)>100)
    {
        Semaphore=2;
        TXBUFF[1]=2;
    }
    if(Metal==1&&((GetADCResult(0)<<2)|ADC_LOW2)>100)
    {
        Semaphore=3;
        TXBUFF[1]=3;
    }
        OLED_ShowNum(5,5,((GetADCResult(0)<<2)|ADC_LOW2),4,16);
}

void remote_control()
{
    if(RXBUFF[0]!=RXBUFF_L[0])//�����ж��յ���ָ���Ƿ�����һ������ͬ����������һ��RXBUFF_L���³���5��ʱ��
    {                           //RXBUFF_L�ڶ�ʱ���ж����������
        comlive_time=com_new;
        
        if(RXBUFF[0]==open)
        {
            //����ͨ��
            //�ϰ����½�
            run=rl;
            comm_pole=pole_down;
            step=7;//
            OLED_ShowString(36,3,"112",16);
        }
        else if(RXBUFF[0]==102)
        {  
            ////��ֹͨ��
            ///
            run=rr;
            comm_pole=pole_up;
            step=0;//
            OLED_ShowString(36,3,"111",16);
            say("Ӣ");//tts����
        }
            *RXBUFF_L=*RXBUFF;
    }
    if(KEY1==0)
    {
        TXBUFF[0]=command1;
    }
    else if(KEY2==0)
    {
        TXBUFF[0]=command2;
    }
}
