#include <reg52.h>
#include <intrins.h>
#include "typedef.h" //为unsigned char和unsigned int添加别名的头文件
#include "interrupt.h"
#include "motor.h"

sbit Wheel_Left_1st = P1^4;
sbit Wheel_Left_2nd = P1^5;
sbit Wheel_Right_2nd = P1^6;
sbit Wheel_Right_1st = P1^7;

void Car_Forward(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 3)
    {
        Wheel_Left(FORWARD);
        Wheel_Right(FORWARD);
    }
    else if (flag_inter0 >= 3 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}

void Car_Back(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 3)
    {
        Wheel_Left(BACK);
        Wheel_Right(BACK);
    }
    else if (flag_inter0 >= 3 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}

void Car_Stop(void)
{
    Wheel_Left(STOP);
    Wheel_Right(STOP);
}

void Car_Left(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 3)
    {
        Wheel_Left(STOP);
        Wheel_Right(FORWARD);
    }
    else if (flag_inter0 >= 3 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}

void Car_Right(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 3)
    {
        Wheel_Left(FORWARD);
        Wheel_Right(STOP);
    }
    else if (flag_inter0 >= 3 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}

void Car_Forward_Faster(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 4)
    {
        Wheel_Left(FORWARD);
        Wheel_Right(FORWARD);
    }
    else if (flag_inter0 >= 4 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}

void Car_Forward_Lower(void)
{
    if (flag_inter0 >= 0 && flag_inter0 < 2)
    {
        Wheel_Left(FORWARD);
        Wheel_Right(FORWARD);
    }
    else if (flag_inter0 >= 2 && flag_inter0 < 5)
    {
        Wheel_Left(STOP);
        Wheel_Right(STOP);
    }
}


void Wheel_Left(char direct)
{
    if (direct == FORWARD)
    {
        Wheel_Left_1st = 1;
        Wheel_Left_2nd = 0;
    }
    else if (direct == BACK)
    {
        Wheel_Left_1st = 0;
        Wheel_Left_2nd = 1;
    }
    else
    {
        Wheel_Left_1st = Wheel_Left_2nd = 0;
    }
}

void Wheel_Right(char direct)
{
    if (direct == FORWARD)
    {
        Wheel_Right_1st = 1;
        Wheel_Right_2nd = 0;
    }
    else if (direct == BACK)
    {
        Wheel_Right_1st = 0;
        Wheel_Right_2nd = 1;
    }
    else
    {
        Wheel_Right_1st = Wheel_Right_2nd = 0;
    }
}