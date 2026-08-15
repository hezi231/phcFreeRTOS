#ifndef __NRF24L01_h__
#define __NRF24L01_h__

#include "NRF24L01_Define.h"
#include <stdio.h>
#include <stdarg.h>
#include "MyRTC.h"  

#define NRF24L01_TX_PACKET_WIDTH								4							//发送数据包宽度，范围：1~32字节
#define NRF24L01_RX_PACKET_WIDTH								4							//接收通道0数据包宽度，范围：1~32字节

#define NRF24L01_Timeout												10

#define NRF24L01_Send_Success										1
#define NRF24L01_Send_Fail											2
#define NRF24L01_Send_Timeout										3

#define	NRF24L01_RxNULL													0
#define	NRF24L01_ReceiveData										1
#define NRF24L01_StatusERROR										2
#define NRF24L01_StillPowerDown									3

void NRF24L01_GPIO_Init(void);
/*协议函数*/
void NRF24L01_SPI_Star(void);
void NRF24L01_SPI_Stop(void);
uint8_t NRF24L01_SPI_SwapByte(uint8_t Byte);
uint8_t NRF24L01_ReadReg(uint8_t RegAddress);
void NRF24L01_ReadRegs(uint8_t RegAddress,uint8_t count,...);
void NRF24L01_WriteReg(uint8_t RegAddress,uint8_t Data);
void NRF24L01_WriteRegs(uint8_t RegAddress,uint8_t count,...);
void NRF24L01_WriteTxPayload(uint8_t *Data,uint8_t count);
void NRF24L01_ReadRxPayload(uint8_t *Data,uint8_t count);

/*功能函数*/
void NRF24L01_Init(void);
uint8_t NRF24L01_Send(uint8_t* TxAddr,uint8_t* Data,uint8_t count);
uint8_t NRF24L01_Receive(uint8_t* Buffer,uint8_t count);

#endif
