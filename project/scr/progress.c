#include "project_config.h"

uchar Semaphore=0;
uint comlive_time=0;
uint rise_time=0;

//Semaphore;//信号量 ：红外和金属检测标志
                       //0：无    1：红外感应到人 2：检测到金属物体
void null()
{
    
}    
void up()
{
   ledr=0;
    ledg=1;
   if(SW2==1)
   {
        step--;
        move(step);
        if(step<=0)
        {
            step=7;
        }
   }
   else
   {
        MotorStop();
        run=MotorStop;
   }
}
void down()
{ 
    ledg=0;
   ledr=1;
   if(SW1==1)
   {
        step++;
        move(step);
        if(step>=7)
        {
            step=0;
        }
   }
   else
   {
        MotorStop();
        rise_time++;
       if(rise_time>keep_time)
       {
           rise_time=0;
           run=up;//run函数指向up函数
       }

   }
}

void sence()
{
    if(Metal==0&&((GetADCResult(0)<<2)|ADC_LOW2)<Pedestrian_distance)//检测到车辆
    {
        Semaphore=vehicle;
        OLED_ShowString(10,0,"Vehicle    ",16);
    }
    else if(Metal==1&&((GetADCResult(0)<<2)|ADC_LOW2)>Pedestrian_distance)//检测到行人
    {
        Semaphore=pedestrian;
        OLED_ShowString(10,0,"Pedestrian",16);
    }
    else if(Metal==0&&((GetADCResult(0)<<2)|ADC_LOW2)>Pedestrian_distance)//都检测到
    {
        Semaphore=all;
        OLED_ShowString(10,0,"ALL       ",16);
    }
    else if(Semaphore==0)
    {
        OLED_ShowString(10,0,"ready      ",16);
    }
    if(KEY1==0)
    {
            OLED_ShowString(10,0,"           ",16);
            ////障碍杆上升
            run=up;//run函数指向up函数
            comm_pole=pole_up;
            OLED_ShowString(10,3,"No Through ",16);
            say("英");//tts语音
    }
    else if(KEY2==0)
    {
            OLED_ShowString(10,0,"           ",16);
            //障碍杆下降
            run=down;//run函数指向down函数
            comm_pole=pole_down;
            OLED_ShowString(10,3,"Let Through   ",16);
            say("英");//tts语音
    }
        TXBUFF[1]=Semaphore;
        OLED_ShowNum(80,5,((GetADCResult(0)<<2)|ADC_LOW2),4,16);
}

void remote_control()
{
    if(RXBUFF[1]!=RXBUFF_L[1])//首先判断收到的指令是否与上一条的相同；当距离上一次RXBUFF_L更新超过5秒时，
    {                           //RXBUFF_L在定时器中断中清空置零
        OLED_ShowString(10,0,"           ",16);
        if(RXBUFF[1]==pole_up)
        {
            ////障碍杆上升
            run=up;//run函数指向up函数
            comm_pole=pole_up;
            OLED_ShowString(10,3,"No Through ",16);
            say("英");//tts语音
        }
        else if(RXBUFF[1]==pole_down)
        {  
            //障碍杆下降
            run=down;//run函数指向up函数
            comm_pole=pole_down;
            OLED_ShowString(10,3,"Let Through   ",16);
            say("英");//tts语音
        }
        else if(RXBUFF[1]==pole_stop)
        {  
            //障碍杆下降
            run=MotorStop;//run函数指向up函数
            comm_pole=pole_stop;
            OLED_ShowString(10,3,"  STOP       ",16);
            say("英");//tts语音
        }
        RXBUFF_L[1]=RXBUFF[1];
    }
}
