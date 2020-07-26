#ifndef __INFRARED_H
#define __INFRARED_H

#include "type.h"
//∫ÏÕ‚º¸¬Î∂®“Â
#define INFRARED_KEY_CHANGE     9
#define INFRARED_KEY_UP         21
#define INFRARED_KEY_DOWN       7
#define INFRARED_KEY_POWER      69
#define INFRARED_KEY_MODE       70

void Infrared_Init(void);
uint8_t Infrared_GetValue(void);

#endif 


