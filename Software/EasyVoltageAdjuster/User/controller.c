/********************************************
文件描述：PID控制
作者：MAGNIN
日志：
2020/06/23 MAGNIN 建立文件

********************************************/
#include "controller.h"
#include "pid.h"
#include "adc.h"
#include "tim.h"

static TypeStructPid vPid;
static TypeStructPid iPid;

static enum EnumPid pidMode = PID_VOLTAGE;
//控制初始化
void Controller_Init(void)
{
  PID_Reset(&vPid);
  vPid.dt = 0.00002;
  vPid.kp = 0.5f;
  vPid.ki = 0.00004f;
  vPid.kd = 0.0f;
  vPid.target = 6.0f;	//V.默认稳压输出
  vPid.LimitMin = 1;
  vPid.LimitMax = 92;
  PID_Init(&vPid);
  
  PID_Reset(&iPid);
  iPid.dt = 0.00002;
  iPid.kp = 10.0f;
  iPid.ki = 0.00010f;
  iPid.kd = 0.0f;
  iPid.target = 0.1f;	//i.默认稳压输出
  iPid.LimitMin = 1;
  iPid.LimitMax = 92;
  PID_Init(&iPid);
  
  TIM1_Init(16,500,0);
}
//PID控制
void PID_Controller(void)
{
  float analog;
  TypePidState state;
  static uint16_t cnt;
  switch(pidMode)
  {
  case PID_VOLTAGE:
    analog = (float)ADC_GetValue(CHANNEL_VOLTAGE)*VOLTAGE_MACRO;
    analog += (float)ADC_GetValue(CHANNEL_CURRENT)*CURRENT_MACRO/10000;  //电流补偿
    PWM_SetDutyCycle((uint16_t)PID_Compute(&vPid,analog));
    break;
  case PID_CURRENT:
    analog = (float)ADC_GetValue(CHANNEL_CURRENT)*CURRENT_MACRO/1000;
    PWM_SetDutyCycle((uint16_t)PID_Compute(&iPid,analog));
    break;
  default:
    break;
  }
  state = PID_GetState();
  if(state == PID_SLOW_START) ++cnt;
  if(cnt > 2000) 
  {
    cnt = 0;
    PID_SetState(PID_NORMAL);
  }
}
//设置控制电压
void PID_SetVoltage(uint8_t voltage)
{
  vPid.target = (float)voltage/10;
}
//设置电流
void PID_SetCurrent(uint8_t currrent)
{
  iPid.target = (float)currrent/100;
}
//设置模式
void PID_SetMode( enum EnumPid mode)
{
  pidMode = mode;
  if(mode == PID_OFF) PWM_Off();
  else PWM_Open();
  
}
//获取模式
enum EnumPid PID_GetMode(void)
{
  return pidMode;
}

