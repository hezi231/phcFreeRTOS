#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "Task_Config.h"

void Task_NRF24L01(void *arg)
{
    while(1)
    {
        vTaskDelay(50);
    }
}
