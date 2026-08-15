#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "Task_Key_Scan.h"
#include "Key.h"
#include "Serial.h"
#include "BlueTooth.h"

void Task_Key_Scan(void *arg)
{
    int8_t pwm = 0;
    int8_t num = 0;
    uint8_t key_num = 0;
    TickType_t delay_time = 10;
    uint32_t run_flag = 0x02;
    while(1)
    {
        Key_Tick();
        key_num = Key_GetNum();
        if(key_num == 1)
        {
            #if (config_ENABLE_DEBUG == 1)
            xTaskNotifyGive(Task_Debugger_Handle);
            #endif
        }
        if(key_num == 2)
        {
            pwm -=10;
            num -=10;
            xQueueOverwrite(motor_pwm_queue,&pwm);
            xQueueSend(key_queue,&num,delay_time);
        }
        if(key_num == 3)
        {
            xTaskNotify(Task_Run_Handle,0x02,eSetBits);
        }
        if(key_num == 4)
        {
            if(run_flag == 0x02)
            {
                run_flag = 0x01;
            }
            else if(run_flag == 0x01)
            {
                run_flag = 0x02;
            }
            xTaskNotify(Task_Run_Handle,run_flag,eSetBits);
        }
        vTaskDelay(10);
    }
}

