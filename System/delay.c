#include "stm32f10x.h"
#include "delay.h"


void delay_Init(void)
{
	DEM_CR |= (uint32_t)DEM_CR_TRCENA;									//使能DWT外设
	DWT_CYCCNT = (uint32_t)0u;													//CYCCNT清零
	DWT_CTRL |= (uint32_t)DWT_CTRL_CYCCNTENA;						//使能CYCCNT计数器
}	

uint32_t DWT_GetCNT(void)
{
	return ((uint32_t)DWT_CYCCNT);
}

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void delay_us(uint32_t xus)
{
	uint32_t Start_Time = DWT_CYCCNT;
	uint32_t Cycles = xus * (CPU_Freq / 1000000u);
	while((DWT_CYCCNT - Start_Time) < Cycles)
	{
	
	}
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_ms(uint32_t xms)
{
	while(xms--)
	{
		delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_s(uint32_t xs)
{
	while(xs--)
	{
		delay_ms(1000);
	}
} 
