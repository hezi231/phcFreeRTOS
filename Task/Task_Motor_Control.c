#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "Motor.h"
#include <math.h>
#include "BlueTooth.h"
#include "PID.h"

PWM_Data_t motor_pwm_receive;
void Task_Motor_Control(void *arg)
{
    while(1)
    {
        if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY) == pdTRUE)
        {
            xQueueReceive(motor_pwm_queue,&motor_pwm_receive,0);
            Motor_Speed(Motor_Left,motor_pwm_receive.LeftPWM);
            Motor_Speed(Motor_Right,motor_pwm_receive.RightPWM);
            if(eTaskGetState(Task_PID_Handle) == eSuspended)
            {

                Motor_Speed(Motor_Left,0);
                Motor_Speed(Motor_Right,0);
                vTaskSuspend(NULL);
                // xTaskNotify(Task_Run_Handle,0x02,eSetBits);
                vTaskDelay(10);
            }
        }
    }
}
