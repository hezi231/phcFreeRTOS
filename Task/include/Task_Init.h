#ifndef __TASK_INIT_h__
#define __TASK_INIT_h__

#include "FreeRTOS.h"
#include "task.h"
#include "Task_Config.h"

/*任务句柄*/
extern TaskHandle_t Task_Hardware_Init_Handle;
extern TaskHandle_t Task_Run_Handle;
extern TaskHandle_t Task_OLED_Handle;
extern TaskHandle_t Task_Key_Scan_Handle;
extern TaskHandle_t Task_MPU6050_Handle;
extern TaskHandle_t Task_Motor_Control_Handle;
extern TaskHandle_t Task_Encoder_Handle;
extern TaskHandle_t Task_PID_Handle;
extern TaskHandle_t Task_Bluetooth_Handle;

extern TaskHandle_t Task_Debugger_Handle;

/*任务配置结构体,配置任务优先级和堆栈大小*/
extern UserTask_t Hardware_Init;
extern UserTask_t Run;
extern UserTask_t OLED_Show;
extern UserTask_t Key_Scan;
extern UserTask_t MPU6050;
extern UserTask_t Motor_Control;
extern UserTask_t Encoder;
extern UserTask_t PID;
extern UserTask_t Bluetooth_UART;
extern TaskHandle_t Task_NRF24L01_Handle;

extern UserTask_t Debugger;
/*队列句柄*/
extern QueueHandle_t key_queue;
extern QueueHandle_t mpu6050_queue;
extern QueueHandle_t motor_pwm_queue;
extern QueueHandle_t motor_speed_queue;
extern QueueHandle_t bluetooth_send_queue;
extern QueueHandle_t nrf24l01_queuek;
extern QueueHandle_t motor_speed_target_queue;

/*软件定时器句柄*/
extern TimerHandle_t soft_timer1_handle;

/*PID结构体*/
extern PID_t AnglePID;
extern PID_t SpeedPID;
extern PID_t TurnPID;

void Task_Init(void);
void Timer1Callback(TimerHandle_t xTimer);

#endif
