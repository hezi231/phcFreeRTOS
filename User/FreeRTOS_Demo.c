#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "Serial.h"
#include "delay.h"
#include "Tool.h"
#include "event_groups.h"


void vApplicationStackOverflowHook(TaskHandle_t xTask,char *pcTaskName)
{
	Serial_Printf("栈溢出");
	while(1)
	{
	
	}
}
