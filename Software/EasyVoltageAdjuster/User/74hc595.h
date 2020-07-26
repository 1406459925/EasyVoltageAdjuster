#ifndef __74HC595_H
#define __74HC595_H
#include "share.h"

#define HC595_SER_SET  PC_ODR_ODR5=1
#define HC595_SER_CLR  PC_ODR_ODR5=0

#define HC595_RCLK_SET  PC_ODR_ODR6=1
#define HC595_RCLK_CLR  PC_ODR_ODR6=0

#define HC595_SRCLK_SET  PC_ODR_ODR7=1
#define HC595_SRCLK_CLR  PC_ODR_ODR7=0


void HC595_Init(void);
void HC595_WriteByte(uint8_t byte);

#endif