#ifndef __DEBUGCONFIG_h__
#define __DEBUGCONFIG_h__

#include "FreeRTOSConfig.h"

// #if (config_ENABLE_DEBUG == 1)
#if (configGENERATE_RUN_TIME_STATS == 1)
	#define deconfigPrint_Run_Time_Stats									1
#endif

#if (configUSE_TRACE_FACILITY == 1)
	#define deconfigPrint_Task_Stats										1
#endif
// #endif

#endif
