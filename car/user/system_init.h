#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#include "sysconfig.h"


void Prepare_ALL(void);
void Time_init(void);

extern int time_ms[3];
extern float way_cm[3];
extern float fre_Cha[3];
extern int time_flag;
extern int way_flag; //路程标志;
extern float time_way; 
#endif

