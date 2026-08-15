#ifndef __Serial_h__
#define __Serial_h__

#include <stdio.h>

extern char Serial_RxPackage[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Data);
void Serial_SendArray(uint16_t *Array);
void Serial_SendString(char *String);
void Serial_SendNum(uint32_t Number);
void Serial_SendSignedNum(int32_t Number);
void Serial_Printf(char *format,...);
uint8_t Serial_GetFlagStatus(void);

#endif
