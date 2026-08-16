#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "Task_OLED.h"
#include "Task_Key_Scan.h"
#include "Task_Hardware_Init.h"
#include "LED.h"
#include "MPU6050.h"
#include "Task_MPU6050.h"
#include "Task_Motor_Control.h"
#include "Task_Encoder.h"
#include "Task_PID.h"
#include "Task_BlueTooth.h"
#include "Task_NRF24L01.h"
#include "Task_Run.h"
#include "Task_Debug.h"

/*硬件任务配置*/
TaskHandle_t Task_Hardware_Init_Handle;
UserTask_t Hardware_Init = {
		.Task_Priority = 30,
		.Task_Stack_Size = 32,
};

/*运行态任务配置*/
TaskHandle_t Task_Run_Handle;
UserTask_t Run = {
		.Task_Priority = 12,
		.Task_Stack_Size = 128,
};

/*OLED显示任务配置*/
TaskHandle_t Task_OLED_Handle;
UserTask_t OLED_Show = {
		.Task_Priority = 2,
		.Task_Stack_Size = 256,
};

/*按键扫描任务配置*/
TaskHandle_t Task_Key_Scan_Handle;
UserTask_t Key_Scan = {
		.Task_Priority = 2,
		.Task_Stack_Size = 64,
};

/*MPU6050任务配置*/
TaskHandle_t Task_MPU6050_Handle;
UserTask_t MPU6050 = {
		.Task_Priority = 29,
		.Task_Stack_Size = 256,
};

/*电机控制任务配置*/
TaskHandle_t Task_Motor_Control_Handle;
UserTask_t Motor_Control = {
		.Task_Priority = 9,
		.Task_Stack_Size = 256,
};

/*编码器任务配置*/
TaskHandle_t Task_Encoder_Handle;
UserTask_t Encoder = {
		.Task_Priority = 29,
		.Task_Stack_Size = 256,
};

/*PID任务配置*/
TaskHandle_t Task_PID_Handle;
UserTask_t PID = {
		.Task_Priority = 11,
		.Task_Stack_Size = 256,
};

/*蓝牙串口任务配置*/	
TaskHandle_t Task_Bluetooth_Handle;
UserTask_t Bluetooth_UART = {
		.Task_Priority = 6,
		.Task_Stack_Size = 256,
};

/*无线任务配置*/	
TaskHandle_t Task_NRF24L01_Handle;
UserTask_t NRF24L01 = {
		.Task_Priority = 8,
		.Task_Stack_Size = 256,
};


#if (config_ENABLE_DEBUG == 1)
/*Debug任务配置,用于串口打印任务状态*/
TaskHandle_t Task_Debugger_Handle;
UserTask_t Debugger = {
		.Task_Priority = 11,
		.Task_Stack_Size = 128,
};
#endif

/*队列配置*/
QueueHandle_t key_queue;
QueueHandle_t mpu6050_queue;
QueueHandle_t motor_pwm_queue;
QueueHandle_t motor_speed_queue;
QueueHandle_t bluetooth_send_queue;
QueueHandle_t nrf24l01_queuek;
QueueHandle_t motor_speed_target_queue;

/*软件定时器配置*/
TimerHandle_t soft_timer1_handle;
void Timer1Callback(TimerHandle_t xTimer);

/*PID结构体配置*/
PID_t AnglePID = {
	.Kp = 2.10,
	.Ki = 0.10,
	.Kd = 4.46,
	.OutOffset = 2,
	.OutMax = 100,
	.OutMin = -100,
};

PID_t SpeedPID = {
	.Kp = 1.24,
	.Ki = 0.02,
	.Kd = 0.0,
	.OutMax = 20,
	.OutMin = -20,
};

PID_t TurnPID = {
	.Kp = 3.00,
	.Ki = 2.45,
	.Kd = 0,
	.OutMax = 50,
	.OutMin = -50,	
};
void Task_Init(void)
{
	/*创建队列*/
	key_queue = xQueueCreate(1,sizeof(uint8_t));
	mpu6050_queue = xQueueCreate(1,sizeof(MPU6050_Data_t));
	motor_pwm_queue = xQueueCreate(1,sizeof(PWM_Data_t));
	motor_speed_queue = xQueueCreate(1,sizeof(Speed_Data_t));
	bluetooth_send_queue = xQueueCreate(1,sizeof(char[100]));
	nrf24l01_queuek = xQueueCreate(1,sizeof(Rocker_t));
	motor_speed_target_queue = xQueueCreate(1,sizeof(Target_Speed_t));
	
	/*创建软件定时器*/
	soft_timer1_handle = xTimerCreate("Timer1", 1000, pdTRUE, (void *)0, Timer1Callback);

	/*创建任务*/
	xTaskCreate(Task_Hardware_Init,
				"Task_Hardware_Init",
				Hardware_Init.Task_Stack_Size,
				NULL,
				Hardware_Init.Task_Priority,
				&Task_Hardware_Init_Handle);
	xTaskCreate(Task_Run,
				"Task_Run",
				Run.Task_Stack_Size,
				NULL,
				Run.Task_Priority,
				&Task_Run_Handle);
	xTaskCreate(Task_OLED,
				"Task_OLED",
				OLED_Show.Task_Stack_Size,
				NULL,
				OLED_Show.Task_Priority,
				&Task_OLED_Handle);
	xTaskCreate(Task_Key_Scan,
				"Task_Key_Scan",
				Key_Scan.Task_Stack_Size,
				NULL,
				Key_Scan.Task_Priority,
				&Task_Key_Scan_Handle);
	xTaskCreate(Task_MPU6050,
				"Task_MPU6050",
				MPU6050.Task_Stack_Size,
				NULL,
				MPU6050.Task_Priority,
				&Task_MPU6050_Handle);
	xTaskCreate(Task_Motor_Control,
				"Task_Motor_Control",
				Motor_Control.Task_Stack_Size,
				NULL,
				Motor_Control.Task_Priority,
				&Task_Motor_Control_Handle);
	xTaskCreate(Task_Encoder,
				"Task_Encoder",
				Encoder.Task_Stack_Size,
				NULL,
				Encoder.Task_Priority,
				&Task_Encoder_Handle);
	xTaskCreate(Task_PID,
				"Task_PID",
				PID.Task_Stack_Size,
				NULL,
				PID.Task_Priority,
				&Task_PID_Handle);	
	xTaskCreate(Task_Bluetooth,
				"Task_Bluetooth",
				Bluetooth_UART.Task_Stack_Size,
				NULL,
				Bluetooth_UART.Task_Priority,
				&Task_Bluetooth_Handle);		
	xTaskCreate(Task_NRF24L01,
				"Task_NRF24L01",
				NRF24L01.Task_Stack_Size,
				NULL,
				NRF24L01.Task_Priority,
				&Task_NRF24L01_Handle);

#if (config_ENABLE_DEBUG == 1)
	xTaskCreate(Task_Debug,
				"Task_Debug",
				Debugger.Task_Stack_Size,
				NULL,
				Debugger.Task_Priority,
				&Task_Debugger_Handle);
#endif		
}

void Timer1Callback(TimerHandle_t xTimer)
{
	LED_Turn();
}
