#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "OLED.h"
#include "MPU6050.h"
#include "Task_Motor_Control.h"

void Task_OLED(void *arg)
{
    Rocker_t receive;
    Target_Speed_t bluetooth_receive;
    while(1)
    {
        xQueueReceive(nrf24l01_queuek,&receive,0);

        OLED_Printf(0,0,OLED_6X8,"LH:%d",receive.LH);
        OLED_Printf(0,9,OLED_6X8,"LV:%d",receive.LV);
        OLED_Printf(0,17,OLED_6X8,"RH:%d",receive.RH);
        OLED_Printf(0,25,OLED_6X8,"RV:%d",receive.RV);
        OLED_UpData();
        xQueueReceive(motor_speed_target_queue,&bluetooth_receive,0);
        OLED_ClearArea(57,0,7 * 8,2 * 16);
        OLED_Printf(47,0,OLED_6X8,"LV:%d",bluetooth_receive.target_speed);
        OLED_Printf(47,9,OLED_6X8,"RH:%d",bluetooth_receive.turn_speed);

        OLED_UpData();        
        vTaskDelay(10);
    }
}
