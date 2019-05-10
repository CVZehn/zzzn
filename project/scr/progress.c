#include "project_config.h"

uchar Semaphore=0;
uint comlive_time=0;
uint rise_time=0;
uchar model=cont_model;
uchar yuyin_last=0;
uint yuyintime =0;

//Semaphore;//�ź��� ������ͽ�������־
                       //0����    1�������Ӧ���� 2����⵽��������
void null()
{
    
}    
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
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
           run=up;//run����ָ��up����
       }

   }
}

////////////////////////////////////////////////////////////////////////////////
void say1()//��������ͨ��
{
    if(yuyin_last!=1)
    {
        yuyin0=1;
        yuyin1=0;
        delay(100);
        yuyin0=1;
        yuyin1=1;
        yuyin_last=1;
        yuyintime=0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void say2()//���Ž�ֹͨ��
{
    if(yuyin_last!=2)
    {
        yuyin0=0;
        yuyin1=1;
        delay(100);
        yuyin0=1;
        yuyin1=1;
        yuyin_last=2;
        yuyintime=0;
    }
}
////////////////////////////////////////////////////////////////////////////////
void sence()
{
    if(Metal==0&&((GetADCResult(0)<<2)|ADC_LOW2)<Pedestrian_distance)//��⵽����
    {
        OLED_ShowString(10,0,"car    ",16);
        if(model==cont_model)
        {
            Semaphore=vehicle;
        }
        else if(model==auto_model)
        {
            run=down;//run����ָ��up����
            say1();//��������
        }
    }
    else if(Metal==1&&((GetADCResult(0)<<2)|ADC_LOW2)>Pedestrian_distance)//��⵽����
    {
        OLED_ShowString(10,0,"people",16);
        if(model==cont_model)
        {
            Semaphore=pedestrian;
        }
        else if(model==auto_model)
        {
            run=down;//run����ָ��up����
            say1();//��������
        }
    }
    else if(Metal==0&&((GetADCResult(0)<<2)|ADC_LOW2)>Pedestrian_distance)//����⵽
    {
        OLED_ShowString(10,0,"ALL       ",16);
        if(model==cont_model)
        {
            Semaphore=all;
        }
        else if(model==auto_model)
        {
            run=down;//run����ָ��up����
            say1();//��������
        }
    }
    else if(Semaphore==0)
    {
        OLED_ShowString(10,0,"ready      ",16);
    }
    if(KEY1==0)
    {
            OLED_ShowString(10,0,"           ",16);
            ////�ϰ�������
            run=up;//run����ָ��up����
            comm_pole=pole_up;
            OLED_ShowString(10,3,"No Through ",16);
    }
    else if(KEY2==0)
    {
            OLED_ShowString(10,0,"           ",16);
            //�ϰ����½�
            run=down;//run����ָ��down����
            comm_pole=pole_down;
            OLED_ShowString(10,3,"Let Through   ",16);
    }
        TXBUFF[1]=Semaphore;
        OLED_ShowNum(80,5,((GetADCResult(0)<<2)|ADC_LOW2),4,16);
}

////////////////////////////////////////////////////////////////////////////////
void remote_control()
{
    if(RXBUFF[1]!=RXBUFF_L[1])//�����ж��յ���ָ���Ƿ�����һ������ͬ��
    {                           //��������һ��RXBUFF_L���³���5��ʱ��RXBUFF_L�ڶ�ʱ���ж����������
        
        if(model==cont_model)
        {
            OLED_ShowString(10,0,"           ",16);
            if(RXBUFF[1]==pole_up)
            {
                say2();//��������
                ////�ϰ�������
                run=up;//run����ָ��up����
                comm_pole=pole_up;
                OLED_ShowString(10,3,"No Through ",16);
                
            }
            else if(RXBUFF[1]==pole_down)
            {  
                say1();//��������
                //�ϰ����½�
                run=down;//run����ָ��up����
                comm_pole=pole_down;
                OLED_ShowString(10,3,"Let Through   ",16);
            }
            else if(RXBUFF[1]==pole_stop)
            {  
                //�ϰ����½�
                run=MotorStop;//run����ָ��up����
                comm_pole=pole_stop;
                OLED_ShowString(10,3,"  STOP       ",16);
            }
        }
        RXBUFF_L[1]=RXBUFF[1];
    }
    model= RXBUFF[0];
    if(model==auto_model)
    {
        OLED_ShowString(0,5,"auto_model",16);
            OLED_ShowString(10,3,"           ",16);
    }
    else if(model==cont_model)
    {
        OLED_ShowString(0,5,"cont_model",16);
    }
}
