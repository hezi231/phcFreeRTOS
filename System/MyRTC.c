#include "stm32f10x.h"                  // Device header
#include "time.h"

uint16_t TIME[] = {2026,5,13,21,7,20};

void MyRTC_SetTime(void);

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
//	if(BKP_ReadBackupRegister(BKP_DR1) != 0x1111)
//	{
		RCC_HSEConfig(RCC_HSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		
		RTC_SetPrescaler(6250 - 1);
		RTC_WaitForLastTask();
		
		MyRTC_SetTime();
//		BKP_WriteBackupRegister(BKP_DR1,0x1111);
//	}
//	else 
//	{
//		RTC_WaitForSynchro();
//		RTC_WaitForLastTask();
//	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt = 0;
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();
}

uint32_t MyRTC_GetSystick(void)
{
	return RTC_GetCounter();
}
