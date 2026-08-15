#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "BlueTooth.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

Target_Speed_t target_speed_send;
Rocker_t rocker_speed_send;
void Task_Bluetooth(void *arg)
{
	uint32_t notify_value[1] = {0};
	char send_string[100];

    while(1)
    {   
		xTaskNotifyWait((uint32_t)0x00, (uint32_t)0xffffffff, notify_value, portMAX_DELAY);
		if(notify_value[0] & 0x01)
		{
			char *p = strtok(BlueTooth_RxPackage,",");
			if(strcmp(p,"s") == 0)
			{
				char *Name = strtok(NULL,",");
				char *Value = strtok(NULL,",");
				if(strcmp(Name,"AngleKp") == 0)
				{
					AnglePID.Kp = atof(Value);
				}
				if(strcmp(Name,"AngleKi") == 0)
				{
					AnglePID.Ki = atof(Value);
				}
				if(strcmp(Name,"AngleKd") == 0)
				{
					AnglePID.Kd = atof(Value);
				}
				if(strcmp(Name,"SpeedKp") == 0)
				{
					SpeedPID.Kp = atof(Value);
				}
				if(strcmp(Name,"SpeedKi") == 0)
				{
					SpeedPID.Ki = atof(Value);
				}
				if(strcmp(Name,"SpeedKd") == 0)
				{
					SpeedPID.Kd = atof(Value);
				}
				// if(strcmp(Name,"TurnKp") == 0)
				// {
				// 	TurnPID.Kp = atof(Value);
				// }
				// if(strcmp(Name,"TurnKi") == 0)
				// {
				// 	TurnPID.Ki = atof(Value);
				// }
				// if(strcmp(Name,"TurnKd") == 0)
				// {
				// 	TurnPID.Kd = atof(Value);
					// BlueTooth_Printf("AngleKp:%3.2f,AngleKi:%3.2f,AngleKd:%3.2f\r\n",AnglePID.Kp,AnglePID.Ki,AnglePID.Kd);
					BlueTooth_Printf("SpeedKp:%3.2f,SpeedKi:%3.2f,SpeedKd:%3.2f\r\n",SpeedPID.Kp,SpeedPID.Ki,SpeedPID.Kd);
				}
			// 	if(strcmp(Name,"Offset") == 0)
			// 	{
			// 		AnglePID.OutOffset = atof(Value);
			// 	}
		// }
			else if(strcmp(p,"j") == 0)
			{
				rocker_speed_send.LH = atof(strtok(NULL,","));
				rocker_speed_send.LV = atof(strtok(NULL,","));
				rocker_speed_send.RH = atof(strtok(NULL,","));
				rocker_speed_send.RV = atof(strtok(NULL,","));			
				target_speed_send.target_speed = rocker_speed_send.LV / 25.0;
				target_speed_send.turn_speed = rocker_speed_send.RH / 20.0;
				
				// SpeedPID.Target = target_speed_send.target_speed;
				xQueueOverwrite(motor_speed_target_queue,&target_speed_send);
			// 	SpeedPID.Target = LV / 25.0;
			// 	TurnPID.Target = RH / 20.0;
			}
				// memset(BlueTooth_RxPackage,0,sizeof(BlueTooth_RxPackage));
		}
		if(notify_value[0] & 0x02)
		{
			xQueueReceive(bluetooth_send_queue,send_string,portMAX_DELAY);
			BlueTooth_SendString(send_string);
		}
            vTaskDelay(10);
    }
}


