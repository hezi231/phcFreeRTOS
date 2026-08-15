#include "stm32f10x.h"                  // Device header
#include "FreeRTOSConfig.h"
#include "Timer.h"

volatile uint32_t FreeRTOS_TimeTick;


void configerTimer(void)
{
	Timer1_Init();
	FreeRTOS_TimeTick = 0;
}

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET)
	{
		FreeRTOS_TimeTick ++;
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
