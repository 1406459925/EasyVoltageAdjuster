/********************************************
�ļ�������PID����
���ߣ�MAGNIN
��־��
2020/06/23 MAGNIN �����ļ�

********************************************/
#include "controller.h"
#include "pid.h"
#include "adc.h"
#include "tim.h"

static TypeStructPid vPid;
static TypeStructPid iPid;

static enum EnumPid pidMode = PID_VOLTAGE;
//���Ƴ�ʼ��
void Controller_Init(void)
{
  PID_Reset(&vPid);
  vPid.dt = 0.00002;
  vPid.kp = 0.5f;
  vPid.ki = 0.00004f;
  vPid.kd = 0.0f;
  vPid.target = 6.0f;	//V.Ĭ����ѹ���
  vPid.LimitMin = 1;
  vPid.LimitMax = 92;
  PID_Init(&vPid);
  
  PID_Reset(&iPid);
  iPid.dt = 0.00002;
  iPid.kp = 10.0f;
  iPid.ki = 0.00010f;
  iPid.kd = 0.0f;
  iPid.target = 0.1f;	//i.Ĭ����ѹ���
  iPid.LimitMin = 1;
  iPid.LimitMax = 92;
  PID_Init(&iPid);
  
  TIM1_Init(16,500,0);
}
//PID����
void PID_Controller(void)
{
  float analog;
  TypePidState state;
  static uint16_t cnt;
  switch(pidMode)
  {
  case PID_VOLTAGE:
    analog = (float)ADC_GetValue(CHANNEL_VOLTAGE)*VOLTAGE_MACRO;
    analog += (float)ADC_GetValue(CHANNEL_CURRENT)*CURRENT_MACRO/10000;  //��������
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
//���ÿ��Ƶ�ѹ
void PID_SetVoltage(uint8_t voltage)
{
  vPid.target = (float)voltage/10;
}
//���õ���
void PID_SetCurrent(uint8_t currrent)
{
  iPid.target = (float)currrent/100;
}
//����ģʽ
void PID_SetMode( enum EnumPid mode)
{
  pidMode = mode;
  if(mode == PID_OFF) PWM_Off();
  else PWM_Open();
  
}
//��ȡģʽ
enum EnumPid PID_GetMode(void)
{
  return pidMode;
}

