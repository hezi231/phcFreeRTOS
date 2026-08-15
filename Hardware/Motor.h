#ifndef __MOTOR_h__
#define __MOTOR_h__

#define Motor_Left						1
#define Motor_Right						2

void Motor_Init(void);
void Motor_Speed(uint8_t Locatoin,int8_t Speed);

#endif
