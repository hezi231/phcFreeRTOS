#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Task_Init.h"
#include <string.h>

char BlueTooth_RxPackage[100];

void BlueTooth_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
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

void USART2_IRQHandler(void)
{
	static uint8_t RxStatus = 0;
	static uint8_t Freq;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		if(RxStatus == 0)
		{
			if(USART_ReceiveData(USART2) == '[')
				{
					RxStatus = 1;
					Freq = 0;
				}
		}
		else if(RxStatus == 1)
		{
			if(USART_ReceiveData(USART2) == ']')
			{
				RxStatus = 0;
				BlueTooth_RxPackage[Freq] = '\0';
				// xQueueSend(bluetooth_receive_queue,BlueTooth_RxPackage,0);
				xTaskNotifyFromISR(Task_Bluetooth_Handle,0x01,eSetBits,NULL);
			}
			else 
			{
			BlueTooth_RxPackage[Freq] = RxData;		
			Freq ++;			
			}
		}		
	}
}

