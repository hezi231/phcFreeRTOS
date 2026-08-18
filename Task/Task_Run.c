#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "PID.h"

void Task_Run(void *arg)
{
    uint32_t notify_value[1] = {0x00};
    while(1)
    {
        xTaskNotifyWait((uint32_t)0x00, (uint32_t)0xffffffff, notify_value, portMAX_DELAY);
        // if(notify_value[0] & 0x03 == 0x03)
        // {     
        //     xTimerChangePeriod(soft_timer1_handle, 1000, 0);
        //     // vTaskSuspend(Task_Motor_Control_Handle);
        //     xTaskNotifyGive(Task_PID_Handle);
        //     vTaskDelay(30);            
        // } 
        if(notify_value[0] & 0x01)
        {
            PID_Init(&AnglePID);
            PID_Init(&SpeedPID);
            PID_Init(&TurnPID);
            xTimerChangePeriod(soft_timer1_handle, 500, 0);
            vTaskResume(Task_PID_Handle);
            vTaskResume(Task_Motor_Control_Handle);
        }
        else if(notify_value[0] & 0x02)
        {
            xTimerChangePeriod(soft_timer1_handle, 1000, 0);
            // vTaskSuspend(Task_Motor_Control_Handle);
            vTaskSuspend(Task_PID_Handle);
            xTaskNotifyGive(Task_Motor_Control_Handle);
            vTaskDelay(30);
        }
    }
}
