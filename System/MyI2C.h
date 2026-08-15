#ifndef __MYI2C_h__
#define __MYI2C_h__

void MyI2C_Init(void);
void MyI2C_Star(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint16_t Data);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t Ack);
uint8_t MyI2C_ReceiveAck(void);

#endif
