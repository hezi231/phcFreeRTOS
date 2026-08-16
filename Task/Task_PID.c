#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "PID.h"
#include "Task_Config.h"
#include <math.h>
#include "BlueTooth.h"

float AngleACC,AngleGyro,Angle;

float Alpha = 0.01;

MPU6050_Data_t receive_data;
PWM_Data_t pwm_send;

Target_Speed_t target_speed_receive;
Speed_Data_t motor_speed_receive;
void Motor_PWM_Init(PWM_Data_t *p)
{
    p->AvePWM = 0;
    p->DifPWM = 0;
    p->LeftPWM = 0;
    p->RightPWM = 0;
}

void Task_PID(void *arg)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    TickType_t DelayTime = 10;
    while(1)
    {
        if(ulTaskNotifyTake(pdTRUE,0) == 1)
        {
            Motor_PWM_Init(&pwm_send);
            xQueueOverwrite(motor_pwm_queue,&pwm_send);
            xTaskNotifyGive(Task_Motor_Control_Handle);
            vTaskSuspend(NULL);
            vTaskDelay(30);
        }
        xQueueReceive(mpu6050_queue,&receive_data,0);

        AngleACC = -atan2(receive_data.ACCx, receive_data.ACCz) / 3.14159 * 180;
        AngleACC += 2.7;
        AngleGyro = Angle + receive_data.GYROy / 32768.0 * 2000 * 0.01;
        Angle = Alpha * AngleACC + (1 - Alpha) * AngleGyro;
        // BlueTooth_Printf("[plot,%f,%f,%f]",AngleACC,AngleGyro,Angle);
        if(Angle > 50 || Angle < -50)
        {
            Motor_PWM_Init(&pwm_send);
            xQueueOverwrite(motor_pwm_queue,&pwm_send);
            xTaskNotifyGive(Task_Motor_Control_Handle);
            // vTaskSuspend(NULL);         
            // xTaskNotify(Task_Run_Handle,0x02,eSetBits);
            vTaskDelay(10);
        }
        AnglePID.Actual = -Angle;
        PID_Updata(&AnglePID);
        pwm_send.AvePWM = AnglePID.Out;
        if(xQueueReceive(motor_speed_queue,&motor_speed_receive,0) == pdTRUE)
        {
            SpeedPID.Actual = motor_speed_receive.AveSpeed;
            PID_Updata(&SpeedPID);
            AnglePID.Target = -SpeedPID.Out;
        }

        pwm_send.LeftPWM = pwm_send.AvePWM + pwm_send.DifPWM / 2.0;
        pwm_send.RightPWM = pwm_send.AvePWM - pwm_send.DifPWM / 2.0;
        // BlueTooth_Printf("[plot,%f,%f]", target_speed_receive.target_speed_receive,SpeedPID.Actual = motor_speed_receive.AveSpeed);
        // BlueTooth_Printf("left_speed:%3.2f,right_speed:%3.2f\r\n", target_speed_receive.target_speed ,target_speed.turn_speed );
        // SpeedPID.Target = target_speed.target_speed;

        xQueueOverwrite(motor_pwm_queue,&pwm_send);
        xTaskNotifyGive(Task_Motor_Control_Handle);
        vTaskDelayUntil(&last_wake_time, DelayTime);
        
    }
}
