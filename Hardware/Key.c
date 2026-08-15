#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t Key_State,Key_Num;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp = 0;
	if(Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
	}
		return Temp;
}

uint8_t Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0)
	{
		return 1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
	{
		return 2;
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
	{
		return 3;
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
	{
		return 4;
	}
	else return 0;
}

void Key_Tick(void)
{
	static uint8_t LastState,NowState;	
	LastState = NowState;
	NowState = Key_GetState();
	if(NowState == 0 && LastState != 0)
	{
		Key_Num = LastState;
	}
}
