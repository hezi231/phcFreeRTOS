#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
#include "delay.h"
#include "BlueTooth.h"  
#include "NRF24L01.h"  
#include "Encoder.h"  
#include "Motor.h"  
#include "MPU6050.h"  
#include "Task_Config.h"

void Task_Hardware_Init(void *arg)
{
	taskENTER_CRITICAL();
	Key_Init();
	OLED_Init();
	delay_Init();
	Serial_Init();
	LED_Init();
	BlueTooth_Init();
	NRF24L01_Init();
	Encoder_Init();
	Motor_Init();
	MPU6050_Init();
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
	vTaskDelay(10);
}

