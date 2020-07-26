#ifndef __PID_H
#define __PID_H
#include "type.h"

typedef struct{
  float target;         //Ŀ��
  float kp;             //��������
  float ki;             //��������
  float kd;             //΢������
  float ap;             //ap = Kp(1 + T/Ti + Td/T)
  float ai;             //ai = Kp(1 + 2Td/T)
  float ad;             //ad = Kp*Td/T
  float dt;             //����ʱ��
  float LimitMax;       //���
  float LimitMin;       //��С
  float error;          //��ǰ���
  float previousError1; //ǰһ�����
  float previousError2; //
  float increment;      //����
  float finalOutput;    //�������
}TypeStructPid;

typedef enum{
  PID_NORMAL,       //����
  PID_PROTECT,      //����
  PID_SLOW_START,   //������
}TypePidState;

void PID_Init(TypeStructPid *pid);
void PID_Clear(TypeStructPid *pid);
void PID_Reset(TypeStructPid *pid);
float PID_Compute(TypeStructPid *pid,float nowValue);

TypePidState PID_GetState(void);
void PID_SetState(TypePidState state);
#endif