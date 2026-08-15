#ifndef __PID_h__
#define __PID_h__

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Task_Config.h"

extern float PID_Parameter[];			//PID_Parameter数组的值为Kp,Ki,Kd,Target



void PID_Init(PID_t *p);
//void PID_SetParameter_v2(PID_t *pid);
void PID_Updata(PID_t *p);

#endif
