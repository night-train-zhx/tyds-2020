
#include "pid.h"
#include "QEI_module.h"


 _PID PID;
 /*
 *���ƣ�position_PID
 *���ܣ�λ��PID
 *ע�⣺ʹ��ǰ���ȳ�ʼ��
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
	PID.SetVal = set;                                 //Ŀ����ٶ�
	PID.ActualVal=angular_velocity;                   //��ǰ���ٶ�
	//���
	PID.err = PID.ActualVal-PID.SetVal;						//��ǰ���ٶ����	
	//������
	PID.integral += PID.err;													//�������
	PID.integral = (PID.integral>  10000) ?  10000:PID.integral;   ////�����޷�
	PID.integral = (PID.integral< -1000) ? -1000:PID.integral;
	PID.voltage   = PID.Kp * PID.err 
								 + PID.Ki * PID.integral 
								 + PID.Kd *(PID.err - PID.err_last);
	//��һ�ε����
	PID.err_last=PID.err;
	return PID.voltage ;
}



 /*
 *���ƣ�PID_realize
 *���ܣ�����ʽPID
 *ע�⣺ʹ��ǰ���ȳ�ʼ��
 */
struct _pid{
    float SetSpeed;            //�����趨ֵ
    float ActualSpeed;        //����ʵ��ֵ
    float err;                //����ƫ��ֵ
    float err_next;            //������һ��ƫ��ֵ
    float err_last;            //��������ǰ��ƫ��ֵ
    float Kp,Ki,Kd;            //������������֡�΢��ϵ��
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
    float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);//ֻ��ǰ�����ε����ֵ�йأ�Ҳ�������
    pid.ActualSpeed+=incrementSpeed;  //ע��Ա���λ��ʽ�����𣬼�����Ľ������Ҫ���ӵģ���λ��ʽ����Ҫ
    pid.err_last=pid.err_next;
    pid.err_next=pid.err;
    return pid.ActualSpeed;
}


