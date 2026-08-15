#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "MPU6050.h"


void Task_MPU6050(void *arg)
{
    MPU6050_Data_t mpu6050_data,mpu6050_data_ave;
    TickType_t last_wake_time = xTaskGetTickCount();
    TickType_t DelayTime = 5;
    uint8_t mpu6050_getTick = 0;
    while(1)
    {
        if(++ mpu6050_getTick >= 5)
        {
            mpu6050_getTick = 0;
            xQueueOverwrite(mpu6050_queue, &mpu6050_data_ave);
            vTaskDelayUntil(&last_wake_time, DelayTime);
        }
        else
        {
            MPU6050_GetData(&mpu6050_data);
            mpu6050_data_ave.ACCx = (mpu6050_data_ave.ACCx * mpu6050_getTick + mpu6050_data.ACCx) / (mpu6050_getTick + 1);
            mpu6050_data_ave.ACCy = (mpu6050_data_ave.ACCy * mpu6050_getTick + mpu6050_data.ACCy) / (mpu6050_getTick + 1);
            mpu6050_data_ave.ACCz = (mpu6050_data_ave.ACCz * mpu6050_getTick + mpu6050_data.ACCz) / (mpu6050_getTick + 1);
            mpu6050_data_ave.GYROx = (mpu6050_data_ave.GYROx * mpu6050_getTick + mpu6050_data.GYROx) / (mpu6050_getTick + 1);
            mpu6050_data_ave.GYROy = (mpu6050_data_ave.GYROy * mpu6050_getTick + mpu6050_data.GYROy) / (mpu6050_getTick + 1);
            mpu6050_data_ave.GYROz = (mpu6050_data_ave.GYROz * mpu6050_getTick + mpu6050_data.GYROz) / (mpu6050_getTick + 1);
        }
        
    }
}
