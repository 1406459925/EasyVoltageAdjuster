#ifndef __CONTROLLER_H
#define __CONTROLLER_H
#include "type.h"

enum EnumPid{
  PID_OFF,
  PID_VOLTAGE,
  PID_CURRENT
};

void Controller_Init(void);
void PID_Controller(void);
void PID_SetVoltage(uint8_t voltage);
void PID_SetCurrent(uint8_t currrent);
void PID_SetMode(enum EnumPid mode);
enum EnumPid PID_GetMode(void);
#endif