#ifndef __BLUETOOTH_h__
#define __BLUETOOTH_h__

extern char BlueTooth_RxPackage[];

void BlueTooth_Init(void);
void BlueTooth_SendByte(uint8_t Data);
void BlueTooth_SendArray(uint16_t *Array,uint8_t Length);
void BlueTooth_SendString(char *String);
void BlueTooth_SendNum(uint32_t Number);
void BlueTooth_SendSignedNum(int32_t Number);
void BlueTooth_Printf(char *format,...);
uint8_t BlueTooth_GetFlagStatus(void);

#endif
