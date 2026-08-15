#ifndef __MPU6050_h__
#define __MPU6050_h__

#include "MPU6050_Reg.h"
#include "Task_Config.h"

void MPU6050_Init(void);
void MPU6050_WriteByte(uint8_t ADDRESS,uint8_t Data);
uint8_t MPU6050_ReadByte(uint8_t ADDRESS);
uint8_t MPU6050_ReadID(void);
void MPU6050_GetData(MPU6050_Data_t *Data);
void MPU6050_ReadArray(uint8_t RegAddress,uint8_t *DataArray,uint8_t Length)
;

#endif
