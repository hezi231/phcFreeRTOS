#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Task_Init.h"
#include <string.h>

#define USART2_DR_BASE					0x40004404
#define BlueTooth_Rx_Buf				100

char BlueTooth_RxPackage[BlueTooth_Rx_Buf];
char DMA_Rx_Buf[BlueTooth_Rx_Buf];
uint8_t BlueTooth_RxFlag = 0;
uint8_t BlueTooth_RxLength = 0;

void BlueTooth_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = BlueTooth_Rx_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_Rx_Buf;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2 -> DR);
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6,ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	USART_Cmd(USART2,ENABLE);
}

void BlueTooth_SendByte(uint8_t Data)
{
	USART_SendData(USART2,Data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void BlueTooth_SendArray(uint16_t *Array,uint8_t Length)
{
	uint8_t i;
	for(i = 0;i < Length;i ++)
	{
		BlueTooth_SendByte(Array[i]); 
	}
}

void BlueTooth_SendString(char *String)
{
	uint8_t i;
	for(i = 0;String[i] != '\0'; i ++)
	{
		BlueTooth_SendByte(String[i]); 		
	}
}

uint32_t BlueTooth_Pow(uint32_t x,uint16_t y)
{
	uint32_t Temp = 1;
	while(y --)
	{
		Temp *= x;
	}
	return Temp;
}

void BlueTooth_SendNum(uint32_t Number)
{
	uint8_t i;
	for(i = sizeof(Number);i > 0;i --)
	{
		BlueTooth_SendByte(Number / BlueTooth_Pow(10,i - 1) % 10 + '0');
	}
}

void BlueTooth_SendSignedNum(int32_t Number)
{
	uint8_t i;
	if(Number > 0)
	{
		BlueTooth_SendByte('+');
	}
	else 
	{
		BlueTooth_SendByte('-');
		Number = -Number;
	}
	for(i = sizeof(Number);i > 0;i --)
	{
		BlueTooth_SendByte(Number / BlueTooth_Pow(10,i - 1) % 10 + '0');
	}
}

void BlueTooth_Printf(char *format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	xQueueSend(bluetooth_send_queue,&String,10);
	xTaskNotify(Task_Bluetooth_Handle,0x02,eSetBits);
}

uint8_t BlueTooth_GetFlagStatus(void)
{
	if(BlueTooth_RxFlag == 1)
	{
		BlueTooth_RxFlag = 0;
		return 1;
	}
	return 0;
}

void USART2_IRQHandler(void)
{
	uint8_t temp = 0;
	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET)
	{
		temp = USART2 ->DR;
		temp = USART2 ->SR;
		(void)temp;
		DMA_Cmd(DMA1_Channel6,DISABLE);
		BlueTooth_RxLength = BlueTooth_Rx_Buf - DMA_GetCurrDataCounter(DMA1_Channel6);
		if(BlueTooth_RxLength > 0)
		{
			memcpy(BlueTooth_RxPackage,DMA_Rx_Buf,BlueTooth_RxLength);
			if(BlueTooth_RxPackage[0] == '[' && BlueTooth_RxPackage[BlueTooth_RxLength - 3] == ']')
			{
				BlueTooth_RxFlag = 1;
				BlueTooth_RxPackage[BlueTooth_RxLength - 3] = '\0';
				DMA_SetCurrDataCounter(DMA1_Channel6,BlueTooth_Rx_Buf);
				DMA_Cmd(DMA1_Channel6,ENABLE);				
			}
			else
			{
				DMA_SetCurrDataCounter(DMA1_Channel6,BlueTooth_Rx_Buf);
				DMA_Cmd(DMA1_Channel6,ENABLE);
			}
		}
		else
		{
			DMA_SetCurrDataCounter(DMA1_Channel6,BlueTooth_Rx_Buf);
			DMA_Cmd(DMA1_Channel6,ENABLE);
		}
	}
	USART_ClearITPendingBit(USART2,USART_IT_IDLE);
}

