
#ifndef PID_PID_H_
#define PID_PID_H_

#include<stdio.h>
#include<stdlib.h>


typedef struct _PID
{
	float SetVal; //�����趨ֵ
	float ActualVal; //����ʵ��ֵ
	float err;      //����ƫ��ֵ
	float err_last; //������һ��ƫ��
	float Kp,Ki,Kd; //�����������΢��
	float voltage; //���		
	float integral; //�������ֵ
}_PID;


extern _PID PID;

extern void PID_init(float Kp,float Ki,float Kd);
extern float  PID_angular_velocity( float angular_velocity,float set);



#endif /* PID_PID_H_ */
