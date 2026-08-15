#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPackage[100];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Data)
{
	USART_SendData(USART1,Data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint16_t *Array)
{
	uint16_t i,Length;
	Length = sizeof(Array);
	for(i = 0;i < Length;i ++)
	{
		Serial_SendByte(Array[i]); 
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for(i = 0;String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]); 		
	}
}

uint32_t Serial_Pow(uint32_t x,uint16_t y)
{
	uint32_t Temp = 1;
	while(y --)
	{
		Temp *= x;
	}
	return Temp;
}

void Serial_SendNum(uint32_t Number)
{
	uint8_t i;
	for(i = sizeof(Number);i > 0;i --)
	{
		Serial_SendByte(Number / Serial_Pow(10,i - 1) % 10 + '0');
	}
}

void Serial_SendSignedNum(int32_t Number)
{
	uint8_t i;
	if(Number > 0)
	{
		Serial_SendByte('+');
	}
	else 
	{
		Serial_SendByte('-');
		Number = -Number;
	}
	for(i = sizeof(Number);i > 0;i --)
	{
		Serial_SendByte(Number / Serial_Pow(10,i - 1) % 10 + '0');
	}
}

int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetFlagStatus(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

void Serial_Printf(char *format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}


//void USART1_IRQHandler(void)
//{
//	static uint8_t RxStatus = 0;
//	static uint8_t Freq;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		if(RxStatus == 0)
//		{
//			if(USART_ReceiveData(USART1) == '@')
//				{
//					RxStatus = 1;
//					Freq = 0;
//				}
//		}
//		else if(RxStatus == 1)
//		{
//			if(USART_ReceiveData(USART1) == '\r')
//			{
//				RxStatus = 2;				
//			}
//			else 
//			{
//			Serial_RxPackage[Freq] = RxData;		
//			Freq ++;			
//			}
//		}
//		else if(RxStatus == 2)
//		{
//			if(USART_ReceiveData(USART1) == '\n')
//				{
//					RxStatus = 0;
//					Serial_RxFlag = 1;
//					Serial_RxPackage[Freq] = '\0';	
//				}
//		}		
//	}
//}

