/********************************************
文件描述：输入按键
作者：MAGNIN
日志：
2020/06/23 MAGNIN 建立文件
2020/06/23 MAGNIN 完成按键驱动编写
********************************************/
#include <iostm8s003f3.h>
#include "key.h"
#include "type.h"
#define  KEY_PRESS        0
#define  KEY_UP_STATUS    PD_IDR_IDR4
#define  KEY_DOWN_STATUS  PD_IDR_IDR5

void KEY_Init(void)
{
  CLR_BIT(PD_DDR,4);  //设置为输入
  CLR_BIT(PD_CR1,4);  //设置为浮空输入模式
  CLR_BIT(PD_CR2,4);  //设置速度为10M
  
  CLR_BIT(PD_DDR,5);  //设置为输入
  CLR_BIT(PD_CR1,5);  //设置为浮空输入模式
  CLR_BIT(PD_CR2,5);  //设置速度为10M
  
}
//按键扫描
TypeEnumKey KEY_Scan(void)
{
  static uint8_t timeDelay = 0;
  static TypeEnumKey key = KEY_NONE;
  switch(key)
  {
  case KEY_NONE:
    if(KEY_UP_STATUS == KEY_PRESS)    key = KEY_UP;   //up按下
    else if(KEY_DOWN_STATUS == KEY_PRESS)  key = KEY_CHANGE; //down按下
    else key = KEY_NONE;
    break;
  case KEY_UP:
    if(timeDelay > KEY_ELIMINATION) //延时到
    {
      key = KEY_NONE;
      timeDelay = 0;
      if(KEY_UP_STATUS == KEY_PRESS)  return KEY_UP;
    }
    else
    {
      timeDelay ++;
      if(KEY_DOWN_STATUS == KEY_PRESS)  key = KEY_SET;
    } 
    break;
  case KEY_CHANGE:
    if(timeDelay > KEY_ELIMINATION) //延时到
    {
      key = KEY_NONE;
      timeDelay = 0;
      if(KEY_DOWN_STATUS == KEY_PRESS)  return KEY_CHANGE;
    }
    else
    {
      timeDelay ++;
      if(KEY_UP_STATUS == KEY_PRESS)  key = KEY_SET;
    }
    break;
  case KEY_SET:
    if(timeDelay > KEY_ELIMINATION) //延时到
    {
      key = KEY_NONE;
      timeDelay = 0;
      if(KEY_UP_STATUS != KEY_PRESS || KEY_DOWN_STATUS != KEY_PRESS)  return KEY_SET;
    }
    else
    {
      timeDelay ++;
    } 
    break;
  default: break;
  }
  return KEY_NONE;
}