/******************************************
创建日期：2020/03/05
文件功能：数码管显示函数
日志：
2020/03/15：创建工程文件
******************************************/
#ifndef __DISPLAY_H
#define __DISPLAY_H
#include <iostm8s003f3.h>
#include "type.h"

#define A1_SET  PC_ODR_ODR3=0
#define A1_CLR  PC_ODR_ODR3=1

#define A2_SET  PB_ODR_ODR4=0
#define A2_CLR  PB_ODR_ODR4=1

#define A3_SET  PB_ODR_ODR5=0 
#define A3_CLR  PB_ODR_ODR5=1

#define DEFAULT_BLINK_TIME  50      //闪烁时间,最大100
#define CLEAR_SEG_1         1
#define CLEAR_SEG_2         2
#define CLEAR_SEG_3         4

enum EnumDisplay{
  OFF_DISPLAY  = 0,
  OPEN_DISPALY = 1
};
void Adress_Clear(void);
void Display_Init(void);
void Display_Clear(uint8_t seg);
void Display_Refresh(void);
void Display_Control(enum EnumDisplay control);
void Display_BlinkSeg(uint8_t seg);
void Display_WriteChar(uint8_t seg,char chr);
void Display_WriteDot(uint8_t dot,uint8_t reverse);
void Display_WriteValue(uint8_t mask,uint16_t value,uint8_t decimal);
void Display_Logo(void);
#endif


