#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "KeyEXIT.h"
#include "BlueTooth.h"
#include "delay.h"
#include "Task_Config.h"
#include "Task_Init.h"

int main(void)
{
	
	Task_Init();
	xTimerStart(soft_timer1_handle,0);
	vTaskResume(Task_PID_Handle);
	vTaskSuspend(Task_Motor_Control_Handle);
	vTaskStartScheduler();
	while(1)
	{

	}
}
