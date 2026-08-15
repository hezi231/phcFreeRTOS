#include "stm32f10x.h"                  // Device header
#include "NRF24L01.h"  
#include "Delay.h"  

uint8_t NRF24L01_RxAddr[5] = {0x11,0x22,0x33,0x44,0x55};

void NRF24L01_W_CE(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)BitValue);
}

void NRF24L01_W_CSN(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)BitValue);
}

void NRF24L01_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)BitValue);
}

void NRF24L01_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)BitValue);
}

uint8_t NRF24L01_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
}

void NRF24L01_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	NRF24L01_W_CE(0);
	NRF24L01_W_CSN(1);
	NRF24L01_W_SCK(0);
	NRF24L01_W_MOSI(0);
}

/*协议函数*/
uint8_t NRF24L01_SPI_SwapByte(uint8_t Byte)
{
	uint8_t ReceiveByte = 0x00;
	for(uint8_t i = 0;i < 8;i ++)
	{
		NRF24L01_W_MOSI(Byte & (0x80 >> i));
		NRF24L01_W_SCK(1);
		if(NRF24L01_R_MISO() == 1)ReceiveByte |= (0x80 >> i);
		NRF24L01_W_SCK(0);
	}
	return ReceiveByte;
}

void NRF24L01_SPI_Star(void)
{
	NRF24L01_W_CSN(0);
}

void NRF24L01_SPI_Stop(void)
{
	NRF24L01_W_CSN(1);
}

uint8_t NRF24L01_ReadReg(uint8_t RegAddress)
{
	uint8_t Temp = 0x00;
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_R_REGISTER | RegAddress);
	Temp = NRF24L01_SPI_SwapByte(0xFF);
	NRF24L01_SPI_Stop();
	return Temp;
}

void NRF24L01_ReadRegs(uint8_t RegAddress,uint8_t count,...)
{
	NRF24L01_SPI_Star();
	uint8_t status = NRF24L01_SPI_SwapByte(NRF24L01_R_REGISTER | RegAddress);
	va_list arg;
	va_start(arg,count);
	for(int i = 0;i < count;i ++)	
	{
		uint8_t data = NRF24L01_SPI_SwapByte(0xFF);
		uint8_t *p = va_arg(arg,uint8_t*);
		if(p != NULL)*p = data;
	}
	NRF24L01_SPI_Stop();
	va_end(arg);	
}

void NRF24L01_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_W_REGISTER | RegAddress);
	NRF24L01_SPI_SwapByte(Data);
	NRF24L01_SPI_Stop();
}

void NRF24L01_WriteRegs(uint8_t RegAddress,uint8_t count,...)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_W_REGISTER | RegAddress);
	va_list arg;
	va_start(arg,count);
	for(int i = 0;i < count;i ++)	
	{
		uint8_t data = (uint8_t) va_arg(arg,int);
		NRF24L01_SPI_SwapByte(data);
	}
	NRF24L01_SPI_Stop();
	va_end(arg);
}

void NRF24L01_WriteTxPayload(uint8_t *Data,uint8_t count)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_W_TX_PAYLOAD);
	for(uint8_t i = 0;i < count;i ++)	
	{
		NRF24L01_SPI_SwapByte(Data[i]);
	}
	NRF24L01_SPI_Stop();
}

void NRF24L01_ReadRxPayload(uint8_t *Data,uint8_t count)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_R_RX_PAYLOAD);
	for(int i = 0;i < count;i ++)	
	{
		Data[i] = NRF24L01_SPI_SwapByte(0xFF);
	}
	NRF24L01_SPI_Stop();
}

void NRF24L01_FlushTxFIFO(void)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_FLUSH_TX);
	NRF24L01_SPI_Stop();
}

void NRF24L01_FlushRxFIFO(void)
{
	NRF24L01_SPI_Star();
	NRF24L01_SPI_SwapByte(NRF24L01_FLUSH_RX);
	NRF24L01_SPI_Stop();
}

uint8_t NRF24L01_ReadStatus(void)
{
	uint8_t status = 0x00;
	NRF24L01_SPI_Star();
	status = NRF24L01_SPI_SwapByte(0xFF);
	NRF24L01_SPI_Stop();
	return status;
}
/*功能函数*/
void NRF24L01_Mode_PowerDowm(void)
{
	uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	config &= ~0x02;
	NRF24L01_WriteReg(NRF24L01_CONFIG,config);
}

void NRF24L01_Mode_StandbyI(void)
{
	uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	config |= 0x02;
	NRF24L01_WriteReg(NRF24L01_CONFIG,config);
	NRF24L01_W_CE(0);
}

void NRF24L01_Mode_Tx(void)
{
	NRF24L01_W_CE(0);
	uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	config |= 0x02;
	config &= ~0x01;
	NRF24L01_WriteReg(NRF24L01_CONFIG,config);
	NRF24L01_W_CE(1);
}

void NRF24L01_Mode_Rx(void)
{
	NRF24L01_W_CE(0);
	
	uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	config |= 0x03;
	NRF24L01_WriteReg(NRF24L01_CONFIG,config);
	NRF24L01_W_CE(1);
}

void NRF24L01_Init(void)
{
	MyRTC_Init();
	NRF24L01_GPIO_Init();
	
	NRF24L01_WriteReg(NRF24L01_CONFIG,0x08);
	NRF24L01_WriteReg(NRF24L01_EN_AA,0x3F);
	NRF24L01_WriteReg(NRF24L01_EN_RXADDR,0x01);
	NRF24L01_WriteReg(NRF24L01_SETUP_AW,0x03);
	NRF24L01_WriteReg(NRF24L01_SETUP_RETR,0x07);
	NRF24L01_WriteReg(NRF24L01_RF_CH,0x55);
	NRF24L01_WriteReg(NRF24L01_RF_SETUP,0x0E);
	NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0,5,NRF24L01_RxAddr[0],NRF24L01_RxAddr[1],NRF24L01_RxAddr[2],
																					 NRF24L01_RxAddr[3],NRF24L01_RxAddr[4]);
	NRF24L01_WriteReg(NRF24L01_RX_PW_P0,NRF24L01_RX_PACKET_WIDTH);

	NRF24L01_Mode_Rx();
	MyRTC_SetTime();
}

uint8_t NRF24L01_Send(uint8_t* TxAddr,uint8_t* Data,uint8_t count)
{
	uint8_t flag = 0;
	NRF24L01_WriteRegs(NRF24L01_TX_ADDR,5,TxAddr[0],TxAddr[1],TxAddr[2],TxAddr[3],TxAddr[4]);
	NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0,5,TxAddr[0],TxAddr[1],TxAddr[2],TxAddr[3],TxAddr[4]);
	NRF24L01_WriteTxPayload(Data,count);
	
	NRF24L01_Mode_Tx();
	
	uint32_t StartTime = MyRTC_GetSystick();
	while(1)
	{
		uint8_t status = NRF24L01_ReadStatus();
		if((status & 0x20) == 0x20)
		{
			flag = NRF24L01_Send_Success;
			break;
		}
		if((status & 0x10) == 0x10)
		{
			flag = NRF24L01_Send_Fail;
			NRF24L01_Init();
			break;
		}
		if(StartTime >= NRF24L01_Timeout)
		{
			flag = NRF24L01_Send_Timeout;
			NRF24L01_Init();
			break;
		}
	}
	NRF24L01_WriteReg(NRF24L01_STATUS,0x30);
	NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0,5,NRF24L01_RxAddr[0],NRF24L01_RxAddr[1],NRF24L01_RxAddr[2],
																					 NRF24L01_RxAddr[3],NRF24L01_RxAddr[4]);
	NRF24L01_FlushTxFIFO();
	NRF24L01_Mode_Rx();
	return flag;
}

uint8_t NRF24L01_Receive(uint8_t* Buffer,uint8_t count)
{
	uint8_t flag = 0;
	uint8_t config = 0;
	uint8_t status = NRF24L01_ReadStatus();
	config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	if ((config & 0x02) == 0x00)			//配置寄存器位1（PWR_UP）为0
	{
		flag = NRF24L01_StillPowerDown;
		NRF24L01_Init();								//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
	}
	else if ((status & 0x30) == 0x30)	//状态寄存器位4（MAX_RT）和位5（TX_DS）同时为1
	{
		flag = NRF24L01_StatusERROR;
		NRF24L01_Init();								//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
	}
	else if((status & 0x40) == 0x40)
	{
		flag = NRF24L01_ReceiveData;
		NRF24L01_ReadRxPayload(Buffer,count);
		NRF24L01_WriteReg(NRF24L01_STATUS,0x40);
		NRF24L01_FlushRxFIFO();		
		return flag;
	}
	return NRF24L01_RxNULL;
}
