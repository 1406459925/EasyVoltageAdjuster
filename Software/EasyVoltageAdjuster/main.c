/*****************************************************
******************USB 调压器 V1.0*********************
描述：仅供学习交流之用，请勿用作商业用途
作者：MAGNIN
版本：V1.0
日志：
2020/06/15 MAGNIN 创建工程
*****************************************************/
#include <iostm8s003f3.h>
#include "intrinsics.h"
#include "tim.h"
#include "adc.h"
#include "menu.h"
#include "share.h"
#include "display.h"
#include "infrared.h"
#include "controller.h"
#include "eeprom.h"
#include "temp.h"

//#define INFRARED_CODE_MEANSURE     //红外码测量   

#define ANALOG_COLLECT_CNT     20  //采集模拟量时间
#define DISPLAY_REFRESH_CNT    40  //显示刷新时间
//初始化时钟
static void Clock_Init(void)
{
  CLK_SWCR_SWIEM = 1;               //开启时钟切换
  CLK_SWR = 0xE1;                   //设置目标时钟源
  while(!MASK_CLK_SWCR_SWIF);       //等待时钟切换完成
  CLK_SWCR &= ~MASK_CLK_SWCR_SWIF;  //清除状态位
  CLK_SWCR_SWEN = 1;                //切换时钟
  CLK_CKDIVR = 0x00;                //设置时钟频率为16MHZ
}
//采集模拟量
static void CollectAnalog(void)
{
  static uint8_t cnt;
  switch(cnt++)
  {
  case 1:
    ADC_INxOpen(CHANNEL_VOLTAGE);
    break;
  case 2:
    ADC_INxConversion(CHANNEL_VOLTAGE);
    ADC_INxOpen(CHANNEL_CURRENT);
    break;
  case 3:
    ADC_INxConversion(CHANNEL_CURRENT);
    ADC_INxOpen(CHANNEL_TEMPERATURE);
    break;
  case 4:
    ADC_INxConversion(CHANNEL_TEMPERATURE);
    break;
  case ANALOG_COLLECT_CNT:
    cnt = 0;
    break;
  default:break;
  }
}
//程序入口
int main( void )
{
  uint16_t i = 0;
  enum EnumCurrentPage page;
  Clock_Init();
  ADC_Init();
  PWM_Init();
  KEY_Init();
  Display_Init();
  Infrared_Init();
  Controller_Init();
  Menu_Init();
  Display_Logo();
  //Delay_Ms(1000);
  PWM_SetDutyCycle(1);
   __enable_interrupt();
  while(1)
  { 
    if(i % 60 == 0)
    {
      TypeEnumKey key = KEY_Scan(); 
      uint8_t inf = Infrared_GetValue();
#ifdef INFRARED_CODE_MEANSURE
      if(inf) Display_WriteValue(0,inf,0);      //红外编码检测
#else
       switch(inf)
      {
      case INFRARED_KEY_MODE:
        key =   KEY_SET;
        break;
      case INFRARED_KEY_CHANGE:
        key =   KEY_CHANGE;
        break;
      case INFRARED_KEY_UP:
        key =  KEY_UP; 
        break;
      case INFRARED_KEY_DOWN:
        key = KEY_DOWN;
        break;
      case  INFRARED_KEY_POWER:
        key = KEY_POWER;
        break;
      default:break;
      }
      page = Menu(key); 
#endif
    } 
#ifndef INFRARED_CODE_MEANSURE
    if(i % 6000 == 0)
    {
       uint16_t value;
       switch(page)
      {
      case CURRENT_VOLTAGE:       //电压显示    0.0V   - 99.9V（显示精度0.1V）
        value = (uint16_t)((float)ADC_GetValue(CHANNEL_VOLTAGE)*10*VOLTAGE_MACRO);
        Display_WriteValue(0,value,1);
        break;
      case CURRENT_CURRENT:       //电流显示范围 0ma - 999ma（显示精度1ma）
        value = (uint16_t)((float)ADC_GetValue(CHANNEL_CURRENT)*CURRENT_MACRO);  //放大11倍
        Display_WriteValue(0,value,0);
        break;
      case  CURRENT_TEMPERATURE:  //温度显示范围 -99℃ - 99.9℃（显示精度-99℃ - -10℃ 内1℃精度，-9.9℃ - 99.9℃内0.1℃精度）
        value = (uint16_t)((float)ADC_GetValue(CHANNEL_TEMPERATURE)*1000*TEMPERATURE_MACRO);
        value = (uint16_t)Calculate_TemperatureValue(value);
        Display_WriteValue(0,(uint16_t)value,1);
        break;
      default:break;
      }
      i = 0;
    } 
#endif
    if(i % DISPLAY_REFRESH_CNT == 0)  Display_Refresh();
    CollectAnalog();
    Delay_Us(16);
    i++;
  }
}

