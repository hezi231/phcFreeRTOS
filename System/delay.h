#ifndef __dELAY_H
#define __dELAY_H

#define DWT_CTRL							*(volatile uint32_t *)0xE0001000			//DWT控制寄存器
#define DWT_CYCCNT						*(volatile uint32_t *)0xE0001004			//CYCCNT周期计数器
#define DEM_CR								*(volatile uint32_t *)0xE000EDFC			//调试异常与监控控制寄存器，用于全局使能或失能跟踪和调试组件

#define DEM_CR_TRCENA 				(1 << 24)					//位24写1,跟踪使能位
#define DWT_CTRL_CYCCNTENA 		(1 << 0)					//位0写1,计数器使能位

#define CPU_Freq							72000000u					//CPU主频

/*
使用DWAT外设的周期计数器来实现延时
*/
void delay_Init(void);					
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_s(uint32_t s);

#endif
