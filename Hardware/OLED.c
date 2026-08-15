#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

uint8_t OLED_InitCommandArray[] = {
0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,
0x12,0x81,0xCF,0xD9,0xF1,0xD8,0x30,0xA4,0xA6,0x8D,0x14,0xAF
};
uint8_t OLED_DispalyBuf[8][128];
uint8_t OLED_ClearArray[128];


void OLED_I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(OLED_SCL_PORT,OLED_SCL_PIN,(BitAction)BitValue);
}

void OLED_I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(OLED_SDA_PORT,OLED_SDA_PIN,(BitAction)BitValue);
}

uint8_t OLED_I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(OLED_SDA_PORT,OLED_SDA_PIN);
	return BitValue;
}

void OLED_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

void OLED_I2C_Star(void)
{
	OLED_I2C_W_SDA(1);
	OLED_I2C_W_SCL(1);
	OLED_I2C_W_SDA(0);
	OLED_I2C_W_SCL(0);
}

void OLED_I2C_Stop(void)
{
	OLED_I2C_W_SDA(0);
	OLED_I2C_W_SCL(1);
	OLED_I2C_W_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Data)
{
	uint8_t i;
	for(i = 0;i < 8;i ++)
	{
		OLED_I2C_W_SDA(Data & (0x80 >> i));
		OLED_I2C_W_SCL(1);
		OLED_I2C_W_SCL(0);
	}
}

uint8_t OLED_I2C_ReceiveByte(void)
{
	uint8_t i;
	uint8_t Data = 0x00;
	OLED_I2C_W_SDA(1);
	for(i = 0;i < 8;i ++)
	{
		OLED_I2C_W_SCL(1);
		if(OLED_I2C_R_SDA() == 1)
		{
			Data |= (0x80 >> i);
		}
		OLED_I2C_W_SCL(0);
	}
	return Data;
}

void OLED_I2C_SendAck(uint8_t Ack)
{
	OLED_I2C_W_SDA(Ack);
	OLED_I2C_W_SCL(1);
	OLED_I2C_W_SCL(0);
}

uint8_t OLED_I2C_ReceiveAck(void)
{
	uint8_t Ack = 0x00;
	OLED_I2C_W_SDA(1);
	OLED_I2C_W_SCL(1);
	Ack = OLED_I2C_R_SDA();
	OLED_I2C_W_SCL(0);
	return Ack;
}


void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Star();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_ReceiveAck();
	OLED_I2C_SendByte(0x00);
	OLED_I2C_ReceiveAck();
	OLED_I2C_SendByte(Command);
	OLED_I2C_ReceiveAck();
	OLED_I2C_Stop();
}

void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Star();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_ReceiveAck();
	OLED_I2C_SendByte(0x40);
	OLED_I2C_ReceiveAck();
	OLED_I2C_SendByte(Data);
	OLED_I2C_ReceiveAck();
	OLED_I2C_Stop();
}

void OLED_WriteCommandArray(uint8_t *CommandArr,uint8_t Length)
{
	uint8_t i;
	OLED_I2C_Star();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_ReceiveAck();	
	OLED_I2C_SendByte(0x00);
	OLED_I2C_ReceiveAck();
	for(i = 0;i < Length;i ++)
	{
		OLED_I2C_SendByte(CommandArr[i]);
		OLED_I2C_ReceiveAck();		
	}
	OLED_I2C_Stop();
}

void OLED_WriteDataArray(uint8_t *DataArr,uint8_t Length)
{
	uint8_t i;
	OLED_I2C_Star();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_ReceiveAck();	
	OLED_I2C_SendByte(0x40);
	OLED_I2C_ReceiveAck();	
	for(i = 0;i < Length;i ++)
	{
		OLED_I2C_SendByte(DataArr[i]);
		OLED_I2C_ReceiveAck();	
	}
	OLED_I2C_Stop();
}

void OLED_SetCursor(uint8_t X,uint8_t Page)
{
	OLED_WriteCommand(0x00 | (X & 0x0F));
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
	OLED_WriteCommand(0xB0 | Page);
}

void OLED_Clear(void)
{
	for(uint8_t j = 0;j < 8;j ++)
	{
		for(uint8_t i = 0;i < 128;i ++)
		OLED_DispalyBuf[j][i] = 0x00;
	}
}

void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t Length,uint8_t Height)
{
	for(uint8_t j = y;j < y + Height;j ++)
	{
		for(uint8_t i = x;i < x + Length;i ++)
		{
			OLED_DispalyBuf[j / 8][i] &= ~(0x01 << (j % 8)); 
		}
	}
}

void OLED_UpData(void)
{
	uint8_t j;
	for(j = 0;j < 8;j ++)
	{
		OLED_SetCursor(0,j);
		OLED_WriteDataArray(OLED_DispalyBuf[j],128);
	}	
}

void OLED_Init(void)
{
	OLED_I2C_Init();
	OLED_WriteCommandArray(OLED_InitCommandArray,23);
	OLED_Clear();
	OLED_UpData();
}

//void OLED_ReverseArea(uint8_t x,uint8_t y,uint8_t Length,uint8_t Height)
//{
//	
//}

void OLED_ShowChar(uint8_t x,uint8_t y,char Char,uint8_t FontSize)
{
	uint8_t clear_height = 0;
	if(FontSize == OLED_8X16)clear_height = 16;
	else clear_height = 8;
	OLED_ClearArea(x,y,FontSize,clear_height);
	if(FontSize == 6)
	{
		OLED_ShowImage(x,y,6,8,OLED_F6x8[Char - ' ']);
	}
	else if(FontSize == 8)
	{
		OLED_ShowImage(x,y,8,16,OLED_F8x16[Char - ' ']);
	}
}	

void OLED_ShowString(uint8_t x,uint8_t y,char *String,uint8_t FontSize)
{
	uint16_t i = 0;
	char SingleChar[5];
	uint8_t CharLength = 0;
	uint16_t XOffset = 0;
	uint16_t pIndex;
	while(String[i] != '\0')
	{
		if((String[i] & 0x80) == 0x00)
		{
			CharLength = 1;
			SingleChar[0] = String[i ++];
			SingleChar[1] = '\0';
		}
		else if((String[i] & 0xE0) == 0xC0)
		{
			CharLength = 2;
			SingleChar[0] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[1] = String[i ++];
			SingleChar[2] = '\0';
		}
		else if((String[i] & 0xF0) == 0xE0)
		{
			CharLength = 3;
			SingleChar[0] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[1] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[2] = String[i ++];
			SingleChar[3] = '\0';
		}
		else if((String[i] & 0xF0) == 0xF0)
		{
			CharLength = 4;
			SingleChar[0] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[1] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[2] = String[i ++];
			if(String[i] == '\0'){break;}
			SingleChar[3] = String[i ++];
			SingleChar[4] = '\0';
		}
		else
		{
			i ++;
			continue;
		}
		if(CharLength == 1)
		{
			OLED_ShowChar(x + XOffset,y,SingleChar[0],FontSize);
			XOffset += FontSize;
		}
		else 
		{
			for(pIndex = 0;strcmp(OLED_CF16x16[pIndex].Index, "") != 0;pIndex ++)
			{
				if(strcmp(OLED_CF16x16[pIndex].Index,SingleChar) == 0)
				{
					break;
				}
			}
			OLED_ShowImage(x + XOffset,y,16,16,OLED_CF16x16[pIndex].Data);
			XOffset += 16;
		}
	}
}

uint32_t OLED_Pow(uint32_t x,uint16_t y)
{
	uint32_t Temp = 1;
	while(y --)
	{
		Temp *= x;
	}
	return Temp;
}

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t Num,uint8_t Length,uint8_t FontSize)
{
	uint8_t i;
	for(i = 0;i < Length;i ++)
	{
		OLED_ShowChar(x + i * FontSize,y,Num / OLED_Pow(10,Length - i - 1) % 10 + '0',FontSize);
	}
}

void OLED_ShowSignedNum(uint8_t x,uint8_t y,int32_t Num,uint8_t Length,uint8_t FontSize)
{
	uint8_t i;
	if(Num >= 0)
	{
		OLED_ShowChar(x,y,'+',FontSize);
		for(i = 0;i < Length;i ++)
		{
			OLED_ShowChar(x + FontSize + i * FontSize,y,Num / OLED_Pow(10,Length - i - 1) % 10 + '0',FontSize);
		}
	}
	else 
	{
		OLED_ShowChar(x,y,'-',FontSize);
		for(i = 0;i < Length;i ++)
		{
			OLED_ShowChar(x + FontSize + i * FontSize,y,(-Num) / OLED_Pow(10,Length - i - 1) % 10 + '0',FontSize);
		}
	}
}

void OLED_ShowHexNum(uint8_t x,uint8_t y,uint32_t Num,uint8_t Length,uint8_t FontSize)
{
	uint8_t i,HexNum;
	
	for(i = 0;i < Length;i ++)
	{
		HexNum = Num / OLED_Pow(16,Length - i - 1) % 16;
		if(HexNum > 9)
		{
			OLED_ShowChar(x + i * FontSize,y,Num / OLED_Pow(16,Length - i - 1) % 16 + '0' + 7,FontSize);	
		}
		else
		{
			OLED_ShowChar(x + i * FontSize,y,Num / OLED_Pow(16,Length - i - 1) % 16 + '0',FontSize);
		}	
	}
}

void OLED_ShowFloatNum(uint8_t x,uint8_t y,float Num,uint8_t inte_Length,uint8_t deci_Length,uint8_t FontSize)
{
	uint32_t temp,i,decimal_temp;
	uint8_t decimal = 0;
	uint8_t decimal_digit = 0;
	temp = Num * OLED_Pow(10,deci_Length);
	
	OLED_ShowNum(x,y,temp / OLED_Pow(10,deci_Length),inte_Length,FontSize);
	
	OLED_ShowChar(x + FontSize * inte_Length,y,'.',FontSize);
	
	for(i = 0;i <= inte_Length + deci_Length;i ++)
	{
		if(i > inte_Length)
		{
			decimal_temp =  temp / OLED_Pow(10,inte_Length + deci_Length - i) % 10;
			decimal = decimal * OLED_Pow(10,decimal_digit) + decimal_temp ;
			decimal_digit ++;
		}
	}
	OLED_ShowNum(x + FontSize * inte_Length + FontSize,y,decimal,deci_Length,FontSize);
}

void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t width,uint8_t height,const uint8_t *Image)
{
	for(uint8_t j = 0;j < ceil(height / 8);j ++)
	{
		for(uint8_t i = 0;i < width;i ++)
		{
			OLED_DispalyBuf[y / 8 + j][x + i] |= (Image[i + j * width] << (y % 8));
			OLED_DispalyBuf[y / 8 + 1 + j][x + i] |= (Image[i + j * width] >> (8 - y % 8));
		}
	}
}

//void OLED_ShowChinese(uint8_t x,uint8_t y,char *Chinese)
//{
//	uint8_t pIndex;
//	uint8_t pChinese = 0;
//	char SigleChinese[4] = {0};
//	for(uint8_t i = 0;Chinese[i] != '\0';i ++)
//	{
//		SigleChinese[pChinese] = Chinese[i];
//		pChinese ++;
//		if(pChinese >= 3)
//		{
//			pChinese = 0;
//			for(pIndex = 0;strcmp(OLED_CF16x16[pIndex].Index, "") != 0;pIndex ++)
//			{
//				if(strcmp(OLED_CF16x16[pIndex].Index,SigleChinese) == 0)
//				{
//					break;
//				}
//			}
//			OLED_ShowImage(x + ((i + 1) / 3 - 1) * 16,y,16,16,OLED_CF16x16[pIndex].Data);
//		}
//	}
//}	

void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...)
{
	char String[256];	
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	OLED_ShowString(X,Y,String,FontSize);
}

void OLED_DrawPoint(uint8_t x,uint8_t y)
{
	OLED_DispalyBuf[y / 8][x] |= 0x01 << (y % 8);
}

uint8_t OLED_GetPoint(uint8_t x,uint8_t y)
{
	return (BitAction)(OLED_DispalyBuf[y / 8][x] & (0x01 << (y % 8)));
}
/*
画线使用bresenham算法，该算法不涉及浮点数运算
*/
void OLED_DrawLine(uint8_t X0,uint8_t Y0,uint8_t X1,uint8_t Y1)
{
	int x,y,dx,dy,incrE,incrNE,d;
	int x0 = X0,y0 = Y0,x1 = X1,y1 = Y1;
	int Temp;
	uint8_t yFlag = 0,xyFlag = 0;
		
	if(x0 == x1)
	{
		if(y0 > y1)
		{
			Temp = y0;
			y0 = y1;
			y1 = Temp;
		}
		for(y = y0;y < y1; y++)
		{
			OLED_DrawPoint(x0,y);
		}
	}
	else if(y0 == y1)
	{
		if(x0 > x1)
		{
			int Temp = x0;
			x0 = x1;
			x1 = Temp;
		}
		for(x = x0;x < x1; x++)
		{
			OLED_DrawPoint(x,y0);
		}		
	}
	else
	{
		if(x1 - x0 < 0)
		{
			Temp = x0;x0 = x1;x1 = Temp;
			Temp = y0;y0 = y1;y1 = Temp;
		}
		if(y1 - y0 < 0)
		{
			y1 = -y1;
			y0 = -y0;
			yFlag = 1;
		}
		if(y1 - y0 > x1 - x0)
		{
			Temp = y0;y0 = x0;x0 = Temp;
			Temp = y1;y1 = x1;x1 = Temp;
			xyFlag = 1;
		}
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		if(yFlag && xyFlag){OLED_DrawPoint(y,-x);}
		else if(yFlag){OLED_DrawPoint(x,-y);}
		else if(xyFlag){OLED_DrawPoint(y,x);}
		for(x = x0 + 1;x <= x1;x ++)
		{
			if(d < 0)
			{
				d += incrE;
			}
			else
			{
				d +=  incrNE;
				y ++;
			}
			if(yFlag && xyFlag){OLED_DrawPoint(y,-x);}
			else if(yFlag){OLED_DrawPoint(x,-y);}
			else if(xyFlag){OLED_DrawPoint(y,x);}
			else {OLED_DrawPoint(x,y);}
		}
	}
}

void OLED_DrawTriangle(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
	OLED_DrawLine(x0,y0,x1,y1);
	OLED_DrawLine(x1,y1,x2,y2);
	OLED_DrawLine(x0,y0,x2,y2);
}

void OLED_DrawRectangle(uint8_t x,uint8_t y,uint8_t Length,uint8_t Height)
{
	OLED_DrawLine(x,y,x + Length,y);
	OLED_DrawLine(x,y,x,y + Height);
	OLED_DrawLine(x + Length,y,x + Length,y + Height);
	OLED_DrawLine(x,y + Height,x + Length,y + Height);
}

/**
	*@brief OLED画四边形
	*@param 左上角的点坐标为x0,y0
	*@param 右上角的点坐标为x1,y1
	*@param 左下角的点坐标为x2,y2
	*@param 右下角的点坐标为x3,y3
	*@return 无
*/
void OLED_DrawQuadrangle(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3)
{
	OLED_DrawLine(x0,y0,x1,y1);
	OLED_DrawLine(x0,y0,x2,y2);
	OLED_DrawLine(x2,y2,x3,y3);
	OLED_DrawLine(x3,y3,x1,y1);
}

