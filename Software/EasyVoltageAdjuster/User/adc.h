#ifndef __ADC_H
#define __ADC_H
#include "type.h"

#define CHANNEL_TEMPERATURE   2
#define CHANNEL_VOLTAGE       3
#define CHANNEL_CURRENT       4

#define CURRENT_MACRO       (3.3/1024*909.1)       
#define VOLTAGE_MACRO       (3.3/1024/10*110) 
#define TEMPERATURE_MACRO   (3.3/1024) 

void ADC_Init(void);
void ADC_INxOpen(uint8_t in);
void ADC_INxConversion(uint8_t in);
uint16_t ADC_GetValue(uint8_t in);

#endif

