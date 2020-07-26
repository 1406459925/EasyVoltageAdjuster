/********************************************
�ļ�������PID
���ߣ�MAGNIN
��־��
2020/06/23 MAGNIN �����ļ�

********************************************/
#include "pid.h"
static TypePidState pidState = PID_NORMAL;
//PID��ʼ��
void PID_Init(TypeStructPid *pid)
{
  pid->ap = pid->kp * (1 + pid->dt / pid->ki + pid->kd / pid->dt);
  pid->ai = pid->kp * (1 + 2 * pid->kd / pid->dt);
  pid->ad = pid->kp * pid->kd / pid->dt;
}
//���PID����
void PID_Clear(TypeStructPid *pid)
{
  pid->error = 0;
  pid->previousError1 = 0;
  pid->previousError2 = 0;
}
//��λPID����
void PID_Reset(TypeStructPid *pid)
{
  pid->target = 0;
  pid->kp = 0;
  pid->ki = 0;
  pid->kd = 0;
  pid->ap = 0;
  pid->ai = 0;
  pid->ad = 0;
  pid->dt = 0;
  pid->LimitMax = 0;
  pid->LimitMin = 0;
  pid->error = 0;
  pid->previousError1 = 0;
  pid->previousError2 = 0;
  pid->increment = 0;
  pid->finalOutput = 0;
}
//PID����
float PID_Compute(TypeStructPid *pid,float nowValue)
{
  pid->error = pid->target - nowValue; //���㵱ǰ���
  pid->increment = pid->ap * pid->error - pid->ai * pid->previousError1 + pid->ad * pid->previousError2;	//����pid
  //�������
  pid->previousError2 = pid->previousError1;
  pid->previousError1 = pid->error;  
  if(pid->increment > 5)
  {
    if(pidState != PID_NORMAL) return pid->finalOutput;
    pidState = PID_SLOW_START;
    pid->finalOutput += 1;
    //pid->finalOutput +=  pid->increment;
  }
  else pid->finalOutput +=  pid->increment;
  
  if(pid->finalOutput > pid->LimitMax)  pid->finalOutput = pid->LimitMax;
  else if(pid->finalOutput < pid->LimitMin)  pid->finalOutput = pid->LimitMin;

  return pid->finalOutput;
}
//��ȡPID״̬
TypePidState PID_GetState(void)
{
  return pidState;
}
//����PID״̬
void PID_SetState(TypePidState state)
{
  pidState = state;
}