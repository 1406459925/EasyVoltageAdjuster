#ifndef __TIM_H
#define __TIM_H
#include "type.h"

#define ARR ((uint16_t)159)
#define TIM1_COUNTERMODE_UP  0x00
#define TIM1_IT_UPDATE       0x01

void TIM1_Init(uint16_t TIM1_Prescaler,uint16_t TIM1_Period,uint8_t TIM1_RepetitionCounter);
void PWM_Init(void);
void PWM_SetDutyCycle( uint16_t TIM2_Pulse);
void PWM_Open(void);
void PWM_Off(void);
uint8_t Get_TimeCount(void);
void Set_TimeCount(uint8_t Count);
#endif

