#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "Tool.h"
#include "Serial.h"
#include "RunTick.h"

#if (config_ENABLE_DEBUG == 1)
void Task_Debug(void *arg)
{ 
    configerTimer();
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        PrintRunTimeStatsSafe();
        PrintTaskStatsSafe();
    }
}
#endif
