#include "stm32f10x.h"                  // Device header
#include "Task_Init.h"
#include "Task_Config.h"
#include "NRF24L01.h"

void Task_NRF24L01(void *arg)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    TickType_t DelayTime = 50;
    uint8_t RXData[NRF24L01_RX_PACKET_WIDTH];
    Rocker_t rocker_send;
    while(1)
    {
        if(NRF24L01_Receive(RXData,4) == NRF24L01_ReceiveData)
        {
            rocker_send.LH = RXData[0];
            rocker_send.LV = RXData[1];
            rocker_send.RH = RXData[2];
            rocker_send.RV = RXData[3];
            xQueueSend(nrf24l01_queuek,&rocker_send,0);
        }
        vTaskDelayUntil(&last_wake_time,DelayTime);
    }
}
