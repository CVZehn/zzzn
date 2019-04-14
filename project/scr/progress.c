#include "project_config.h"

uchar Semaphore=0;
u8 comlive_time;

//Semaphore;//信号量 ：红外和金属检测标志
                       //0：无    1：红外感应到人 2：检测到金属物体
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
    if(RXBUFF[0]!=RXBUFF_L[0])//首先判断收到的指令是否与上一条的相同；当距离上一次RXBUFF_L更新超过5秒时，
    {                           //RXBUFF_L在定时器中断中清空置零
        comlive_time=com_new;
        
        if(RXBUFF[0]==open)
        {
            //允许通过
            //障碍杆下降
            run=rl;
            comm_pole=pole_down;
            step=7;//
            OLED_ShowString(36,3,"112",16);
        }
        else if(RXBUFF[0]==102)
        {  
            ////禁止通过
            ///
            run=rr;
            comm_pole=pole_up;
            step=0;//
            OLED_ShowString(36,3,"111",16);
            say("英");//tts语音
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
