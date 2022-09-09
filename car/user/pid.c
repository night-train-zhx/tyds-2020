
#include "pid.h"
#include "QEI_module.h"


 _PID PID;
 /*
 *名称：position_PID
 *功能：位置PID
 *注意：使用前请先初始化
 */
 
void PID_init(float Kp,float Ki,float Kd)
{
	PID.SetVal=0;
	PID.ActualVal=0;
	PID.err=0;
	PID.err_last=0;
	PID.integral=0;
	
	PID.Kp= Kp ;
	PID.Ki= Ki ;
	PID.Kd= Kd;
}


float position_PID( float angular_velocity,float set)
{
	PID.SetVal = set;                                 //目标角速度
	PID.ActualVal=angular_velocity;                   //当前角速度
	//误差
	PID.err = PID.ActualVal-PID.SetVal;						//当前角速度误差	
	//误差积分
	PID.integral += PID.err;													//积分误差
	PID.integral = (PID.integral>  10000) ?  10000:PID.integral;   ////积分限幅
	PID.integral = (PID.integral< -1000) ? -1000:PID.integral;
	PID.voltage   = PID.Kp * PID.err 
								 + PID.Ki * PID.integral 
								 + PID.Kd *(PID.err - PID.err_last);
	//上一次的误差
	PID.err_last=PID.err;
	return PID.voltage ;
}



 /*
 *名称：PID_realize
 *功能：增量式PID
 *注意：使用前请先初始化
 */
struct _pid{
    float SetSpeed;            //定义设定值
    float ActualSpeed;        //定义实际值
    float err;                //定义偏差值
    float err_next;            //定义上一个偏差值
    float err_last;            //定义最上前的偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
}pid;

void PID2_init(){
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.err_next=0.0;
    pid.Kp=0.2;
    pid.Ki=0.0;
    pid.Kd=0.0;
}

float PID_realize(float speed){
    pid.SetSpeed=speed;
    pid.err=pid.SetSpeed-pid.ActualSpeed;
    float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);//只和前后三次的误差值有关，也方便计算
    pid.ActualSpeed+=incrementSpeed;  //注意对比与位置式的区别，计算出的结果是需要叠加的，而位置式不需要
    pid.err_last=pid.err_next;
    pid.err_next=pid.err;
    return pid.ActualSpeed;
}


