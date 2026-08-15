#ifndef __TASK_CONFIG_h__
#define __TASK_CONFIG_h__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"

/*任务结构体*/
typedef struct {
	uint8_t Task_Priority;
	uint16_t Task_Stack_Size;
}UserTask_t;

/*MPU6050数据结构体*/
typedef struct
{
    int16_t ACCx;
    int16_t ACCy;
    int16_t ACCz;
    int16_t GYROx;
    int16_t GYROy;
    int16_t GYROz;
}MPU6050_Data_t;

/*编码器结构体*/
typedef struct {
	int16_t Encoder_Left;
	int16_t Encoder_Right;
}Encoder_Data_t;

/*电机速度结构体*/
typedef struct {
	float Left_Speed;
	float Right_Speed;
	float AveSpeed;
	float DifSpeed;
}Speed_Data_t;

/*PWM结构体*/
typedef struct {
	int8_t LeftPWM;
	int8_t RightPWM;
	int8_t AvePWM;
	int8_t DifPWM;
}PWM_Data_t;

/*摇杆结构体*/
typedef struct {
	int8_t LH;
	int8_t LV;
	int8_t RH;
	int8_t RV;
}Rocker_t;

/*速度设置结构体*/
typedef struct {
	float target_speed;
	float turn_speed;
}Target_Speed_t;

/*PID结构体*/
typedef struct{
	float Target;
	float Actual;
	float Actual2;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	float Diout;
	
	float OutMax;
	float OutMin;
	
	float OutOffset;
	
}PID_t;

#endif
