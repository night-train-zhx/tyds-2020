
#ifndef PID_PID_H_
#define PID_PID_H_

#include<stdio.h>
#include<stdlib.h>


typedef struct _PID
{
	float SetVal; //定义设定值
	float ActualVal; //定义实际值
	float err;      //定义偏差值
	float err_last; //定义上一个偏差
	float Kp,Ki,Kd; //定义比例积分微分
	float voltage; //输出		
	float integral; //定义积分值
}_PID;


extern _PID PID;

extern void PID_init(float Kp,float Ki,float Kd);
extern float  PID_angular_velocity( float angular_velocity,float set);



#endif /* PID_PID_H_ */
