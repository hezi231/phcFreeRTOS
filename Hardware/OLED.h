#ifndef __OLED_h__
#define __OLED_h__

#include "OLED_Matrix.h"

#define OLED_8X16				8
#define OLED_6X8				6
#define OLED_SCL_PORT		GPIOB
#define OLED_SCL_PIN		GPIO_Pin_8
#define OLED_SDA_PORT		GPIOB
#define OLED_SDA_PIN		GPIO_Pin_9

extern uint8_t OLED_DispalyBuf[8][128];
/*
由于是使用带缓存区的OLED显示，因此在调用显示函数时只是对缓存区数组进行操作，要想显示到OLED上，
需要调用更新函数OLED_UpData。
*/

void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);
void OLED_WriteCommandArray(uint8_t *CommandArr,uint8_t Length);
void OLED_WriteDataArray(uint8_t *DataArr,uint8_t Length);
void OLED_SetCursor(uint8_t X,uint8_t Page);
void OLED_Clear(void);
void OLED_UpData(void);
void OLED_Init(void);

/*
x为横坐标，范围0~128，y为纵坐标，范围0~64
*/

void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t Length,uint8_t Height);
void OLED_ShowChar(uint8_t x,uint8_t y,char Char,uint8_t FontSize);
void OLED_ShowString(uint8_t x,uint8_t y,char *String,uint8_t FontSize);
uint32_t OLED_Pow(uint32_t x,uint16_t y);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t Num,uint8_t Length,uint8_t FontSize);
void OLED_ShowSignedNum(uint8_t x,uint8_t y,int32_t Num,uint8_t Length,uint8_t FontSize);
void OLED_ShowHexNum(uint8_t x,uint8_t y,uint32_t Num,uint8_t Length,uint8_t FontSize);
void OLED_ShowFloatNum(uint8_t x,uint8_t y,float Num,uint8_t inte_Length,uint8_t deci_Length,uint8_t FontSize);
void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t width,uint8_t height,const uint8_t *Image);
//void OLED_ShowChinese(uint8_t x,uint8_t y,char *Chinese);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

void OLED_DrawPoint(uint8_t x,uint8_t y);
uint8_t OLED_GetPoint(uint8_t x,uint8_t y);
void OLED_DrawLine(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1);
void OLED_DrawTriangle(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawQuadrangle(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3);
void OLED_DrawRectangle(uint8_t x,uint8_t y,uint8_t Length,uint8_t Height);

#endif
