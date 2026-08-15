#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Task_Init.h"

void Task_Encoder(void *arg)
{
    Speed_Data_t Speed_Data_Send;
    Encoder_Data_t Encoder_Data;
    TickType_t last_wake_time = xTaskGetTickCount();
    TickType_t DelayTime = 50;
    while(1)
    {
        Encoder_Get(&Encoder_Data);
        Speed_Data_Send.Left_Speed = Encoder_Data.Encoder_Left / 44.0 / 0.05 / 9.27666;
        Speed_Data_Send.Right_Speed = Encoder_Data.Encoder_Right / 44.0 / 0.05 / 9.27666;
        Speed_Data_Send.AveSpeed = (Speed_Data_Send.Left_Speed + Speed_Data_Send.Right_Speed) / 2;
        Speed_Data_Send.DifSpeed = Speed_Data_Send.Left_Speed - Speed_Data_Send.Right_Speed;
        xQueueOverwrite(motor_speed_queue,&Speed_Data_Send);
        vTaskDelayUntil(&last_wake_time, DelayTime);
    }
}
