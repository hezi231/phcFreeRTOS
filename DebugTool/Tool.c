#include "stm32f10x.h"                  // Device header
#include "Tool.h"
#include "FreeRTOS.h"
#include "Serial.h"
#include "task.h"
#include <stdio.h>
#include "Debugconfig.h"
/**
	*@brief 打印所有任务状态,仅适用于调试
	*@param 无
	*@return 无
*/
#if (deconfigPrint_Task_Stats == 1)
void PrintTaskStatsSafe(void)
{
	/*获取任务数量*/
	UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();
	TaskStatus_t* pxTaskStatusArray;
	/*分配任务状态数组*/
	pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
	/*获取原始状态数据*/
	uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,uxArraySize,NULL);
	/*手动打印字符*/
	Serial_Printf("\r\n--------------- Task Stats ---------------\r\n");
	Serial_Printf("任务名\t\t任务状态\t\t任务优先级\t任务堆栈最小值\t\t任务编号\r\n");
	
	for(UBaseType_t i = 0; i < uxArraySize; i++)
	{		
		// 使用简单的 printf 逐个打印，栈消耗极小
		Serial_Printf("%-12s\t%d\t\t%ld\t\t%-4ld\t\t\t%ld\r\n", 
					 pxTaskStatusArray[i].pcTaskName, 
					 pxTaskStatusArray[i].eCurrentState,
					 pxTaskStatusArray[i].uxCurrentPriority,
					 pxTaskStatusArray[i].usStackHighWaterMark,
					 pxTaskStatusArray[i].xTaskNumber);
	}
	
	Serial_Printf("----------------------------------------------\r\n");
	vPortFree(pxTaskStatusArray);
}
#endif

/**
	*@brief 打印所有任务运行时间,仅适用于调试
	*@param 无
	*@return 无
*/
#if (deconfigPrint_Run_Time_Stats == 1)
void PrintRunTimeStatsSafe(void)
{
	// 1. 获取当前系统任务数量
	UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();
	TaskStatus_t* pxTaskStatusArray;
	// 2. 动态或静态分配任务状态数组 (注意栈空间，推荐用 static 或 malloc)
	pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
	// 3. 获取系统总运行时间 (需要你自己实现获取计数器当前值的宏)
	uint32_t ulTotalRunTime = portGET_RUN_TIME_COUNTER_VALUE();	
	// 4. 获取原始状态数据 (不消耗大量栈，不依赖 sprintf)
	uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
	// 5. 手动格式化打印 (按需打印，完全可控)
	Serial_Printf("\r\n--- CPU Run Time Stats ---\r\n");
	Serial_Printf("Task\t\tAbs\t\t\t%%\r\n");
	
	for(UBaseType_t i = 0; i < uxArraySize; i++)
	{
		uint32_t ulStatsAsPercentage;
		
		// 防止除零异常！
		if(ulTotalRunTime == 0)
		{
			ulStatsAsPercentage = 0;
		}
		else
		{
			ulStatsAsPercentage = (pxTaskStatusArray[i].ulRunTimeCounter * 100) / ulTotalRunTime;
		}
		
		// 使用简单的 printf 逐个打印，栈消耗极小
		Serial_Printf("%-12s\t%u\t\t\t%u%%\r\n", 
					 pxTaskStatusArray[i].pcTaskName, 
					 (unsigned int)pxTaskStatusArray[i].ulRunTimeCounter, 
					 (unsigned int)ulStatsAsPercentage);
	}
	Serial_Printf("--------------------------\r\n");
	vPortFree(pxTaskStatusArray);
}
#endif
