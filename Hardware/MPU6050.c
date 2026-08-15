#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050.h"
#include "Task_Config.h"

#define MPU6050_ADDRESS				0xD0

void MPU6050_WriteByte(uint8_t RegAddress,uint8_t Data)
{
	MyI2C_Star();
	MyI2C_SendByte(0xD0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadByte(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Star();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Star();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data =  MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_ReadArray(uint8_t RegAddress,uint8_t *DataArray,uint8_t Length)
{	
	MyI2C_Star();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Star();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	for(uint8_t i = 0;i < Length;i ++)
	{
		DataArray[i] = MyI2C_ReceiveByte();
		if(i != Length - 1)MyI2C_SendAck(0);
		else MyI2C_SendAck(1);
	}
	MyI2C_Stop();
	
}

void MPU6050_Init(void)
{ 
	MyI2C_Init();
	MPU6050_WriteByte(MPU6050_PWR_MGMT_1,0x01);
	MPU6050_WriteByte(MPU6050_PWR_MGMT_2,0x00);
 	MPU6050_WriteByte(MPU6050_SMPLRT_DIV,0x00);
 	MPU6050_WriteByte(MPU6050_CONFIG,0x01);
 	MPU6050_WriteByte(MPU6050_GYRO_CONFIG,0x18);
 	MPU6050_WriteByte(MPU6050_ACCEL_CONFIG,0x18);
}

uint8_t MPU6050_ReadID(void)
{
	return MPU6050_ReadByte(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(MPU6050_Data_t *Data)
{
	uint8_t Array[14];
	MPU6050_ReadArray(MPU6050_ACCEL_XOUT_H,Array,14);
	
	Data->ACCx = (Array[0] << 8) | Array[1];
	Data->ACCy = (Array[2] << 8) | Array[3];
	Data->ACCz = (Array[4] << 8) | Array[5];

	Data->GYROx = (Array[8] << 8) | Array[9];
	Data->GYROy = (Array[10] << 8) | Array[11];
	Data->GYROz = (Array[12] << 8) | Array[13];
}
