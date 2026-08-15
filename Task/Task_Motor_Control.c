#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "Motor.h"
#include <math.h>
#include "BlueTooth.h"
#include "PID.h"

// float AngleACC,AngleGyro,Angle;

// float AvePWM,DifPWM;
// float LeftPWM,RightPWM;

// Speed_Data_t motor_speed_receive;

// void Task_Motor_Control(void *arg)
// {
//     TickType_t last_wake_time = xTaskGetTickCount();
//     TickType_t DelayTime = 10;
//     MPU6050_Data_t receive_data;
//     float Alpha = 0.01;
//     while(1)
//     {
//         if(ulTaskNotifyTake(pdTRUE,0) == 1)
//         {
//             Motor_Speed(Motor_Right,0);
//             Motor_Speed(Motor_Left,0);
//             vTaskSuspend(NULL);
//             vTaskDelay(30);    
//         }
//         xQueueReceive(mpu6050_queue,&receive_data,0);
//         AngleACC = -atan2(receive_data.ACCx, receive_data.ACCz) / 3.14159 * 180;
//         AngleACC += 2.7;
//         AngleGyro = Angle + receive_data.GYROy / 32768.0 * 2000 * 0.01;
//         Angle = Alpha * AngleACC + (1 - Alpha) * AngleGyro;
//         // BlueTooth_Printf("[plot,%f,%f,%f]",AngleACC,AngleGyro,Angle);
//         if(Angle > 50 || Angle < -50)
//         {
//             xTaskNotifyGive(Task_Run_Handle);
//         }
//         AnglePID.Actual = -Angle;
//         PID_Updata(&AnglePID);
//         AvePWM = AnglePID.Out;
//         LeftPWM = AvePWM + DifPWM / 2.0;
//         RightPWM = AvePWM - DifPWM / 2.0;

//         if(LeftPWM > 100){LeftPWM = 100;}else if(LeftPWM < -100){LeftPWM = -100;}
//         if(RightPWM > 100){RightPWM = 100;}else if(RightPWM < -100){RightPWM = -100;}

//         Motor_Speed(Motor_Left,LeftPWM);
//         Motor_Speed(Motor_Right,RightPWM);      
        
//         if(xQueueReceive(motor_speed_queue,&motor_speed_receive,0) == pdTRUE)
//         {
//             SpeedPID.Actual = motor_speed_receive.AveSpeed;
//             PID_Updata(&SpeedPID);
//             AnglePID.Target = -SpeedPID.Out;
//         }
//         vTaskDelayUntil(&last_wake_time, DelayTime);
//     }
// }

PWM_Data_t motor_pwm_receive;
void Task_Motor_Control(void *arg)
{
    while(1)
    {
        if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY) == pdTRUE)
        {
            xQueueReceive(motor_pwm_queue,&motor_pwm_receive,0);
            Motor_Speed(Motor_Left,motor_pwm_receive.LeftPWM);
            Motor_Speed(Motor_Right,motor_pwm_receive.RightPWM);
            if(eTaskGetState(Task_PID_Handle) == eSuspended)
            {

                Motor_Speed(Motor_Left,0);
                Motor_Speed(Motor_Right,0);
                vTaskSuspend(NULL);
                // xTaskNotify(Task_Run_Handle,0x02,eSetBits);
                vTaskDelay(10);
            }
        }
    }
}
