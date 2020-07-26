#ifndef __PID_H
#define __PID_H
#include "type.h"

typedef struct{
  float target;         //目标
  float kp;             //比例增益
  float ki;             //积分增益
  float kd;             //微分增益
  float ap;             //ap = Kp(1 + T/Ti + Td/T)
  float ai;             //ai = Kp(1 + 2Td/T)
  float ad;             //ad = Kp*Td/T
  float dt;             //积分时间
  float LimitMax;       //最大
  float LimitMin;       //最小
  float error;          //当前误差
  float previousError1; //前一次误差
  float previousError2; //
  float increment;      //增量
  float finalOutput;    //最终输出
}TypeStructPid;

typedef enum{
  PID_NORMAL,       //正常
  PID_PROTECT,      //保护
  PID_SLOW_START,   //缓启动
}TypePidState;

void PID_Init(TypeStructPid *pid);
void PID_Clear(TypeStructPid *pid);
void PID_Reset(TypeStructPid *pid);
float PID_Compute(TypeStructPid *pid,float nowValue);

TypePidState PID_GetState(void);
void PID_SetState(TypePidState state);
#endif