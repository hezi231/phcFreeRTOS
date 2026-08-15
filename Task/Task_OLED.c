#include "stm32f10x.h"                  // Device header
#include "Task_Config.h"
#include "Task_Init.h"
#include "OLED.h"
#include "MPU6050.h"
#include "Task_Motor_Control.h"

void Task_OLED(void *arg)
{
    /*按键测试*/
    //int8_t receive = 0;
    // while (1)
    // {
    //     xQueueReceive(key_queue,&receive,portMAX_DELAY);
    //     OLED_Printf(0,0,OLED_8X16,"%d",receive);
    //     OLED_UpData();
    //     vTaskDelay(10);
    // }

    /*MPU6050测试*/
    // MPU6050_Data_t mpu6050_data;
    // int8_t receive = 0;
    // while (1)
    // {
    //     OLED_Clear();
    //     xQueuePeek(mpu6050_queue, &mpu6050_data, portMAX_DELAY);
    //     OLED_ShowSignedNum(0,0,mpu6050_data.ACCx,5,OLED_8X16);
	// 	OLED_ShowSignedNum(0,17,mpu6050_data.ACCy,5,OLED_8X16);
	// 	OLED_ShowSignedNum(0,33,mpu6050_data.ACCz,5,OLED_8X16);
	// 	OLED_ShowSignedNum(6 * 8 + 1,0,mpu6050_data.GYROx,5,OLED_8X16);
	// 	OLED_ShowSignedNum(6 * 8 + 1,17,mpu6050_data.GYROy,5,OLED_8X16);
	// 	OLED_ShowSignedNum(6 * 8 + 1,33,mpu6050_data.GYROz,5,OLED_8X16);

    //     xQueueReceive(key_queue,&receive,0);
    //     OLED_Printf(0,49,OLED_8X16,"%d",receive);
    //     OLED_UpData();
    //     vTaskDelay(10);
    // }

    /*编码器测试*/
    // Encoder_Data_t encoder_data;
    // int8_t receive = 0;

    // while (1)
    // {
    //     OLED_Clear();
    //     xQueueReceive(key_queue,&receive,0);
    //     xQueueReceive(encouder_queue, &encoder_data, portMAX_DELAY);
    //     OLED_ShowSignedNum(0,0,encoder_data.Encoder_Left,5,OLED_8X16);
    //     OLED_ShowSignedNum(0,17,encoder_data.Encoder_Right,5,OLED_8X16);
    //     OLED_Printf(0,49,OLED_8X16,"%d",receive);
    //     OLED_UpData();
    // }

    /*蓝牙测试*/
    while(1)
    {
        OLED_Clear();
    	OLED_Printf(0,0,OLED_6X8,"Kp:%3.2f",AnglePID.Kp);
		OLED_Printf(0,9,OLED_6X8,"Kp:%3.2f",AnglePID.Ki);
		OLED_Printf(0,17,OLED_6X8,"Kp:%3.2f",AnglePID.Kd);
		OLED_Printf(0,25,OLED_6X8,"Tar:%5.2f",AnglePID.Target);
		// OLED_Printf(0,33,OLED_6X8,"Act:%5.2f",Angle);
		// OLED_Printf(0,25,OLED_6X8,"Tar:%5.2f",AnglePID.Target);
		// OLED_Printf(0,33,OLED_6X8,"Act:%5.2f",Angle);
		// OLED_Printf(0,41,OLED_6X8,"Out:%5.2f",AnglePID.Out);
        OLED_UpData();
        vTaskDelay(5);
    }
}
