/*****************************************************
******************USB ��ѹ�� V1.0*********************
����������ѧϰ����֮�ã�����������ҵ��;
���ߣ�MAGNIN
�汾��V1.0
��־��
2020/06/15 MAGNIN ��������
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

//#define INFRARED_CODE_MEANSURE     //���������   

#define ANALOG_COLLECT_CNT     20  //�ɼ�ģ����ʱ��
#define DISPLAY_REFRESH_CNT    40  //��ʾˢ��ʱ��
//��ʼ��ʱ��
static void Clock_Init(void)
{
  CLK_SWCR_SWIEM = 1;               //����ʱ���л�
  CLK_SWR = 0xE1;                   //����Ŀ��ʱ��Դ
  while(!MASK_CLK_SWCR_SWIF);       //�ȴ�ʱ���л����
  CLK_SWCR &= ~MASK_CLK_SWCR_SWIF;  //���״̬λ
  CLK_SWCR_SWEN = 1;                //�л�ʱ��
  CLK_CKDIVR = 0x00;                //����ʱ��Ƶ��Ϊ16MHZ
}
//�ɼ�ģ����
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
//�������
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
      if(inf) Display_WriteValue(0,inf,0);      //���������
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
      case CURRENT_VOLTAGE:       //��ѹ��ʾ    0.0V   - 99.9V����ʾ����0.1V��
        value = (uint16_t)((float)ADC_GetValue(CHANNEL_VOLTAGE)*10*VOLTAGE_MACRO);
        Display_WriteValue(0,value,1);
        break;
      case CURRENT_CURRENT:       //������ʾ��Χ 0ma - 999ma����ʾ����1ma��
        value = (uint16_t)((float)ADC_GetValue(CHANNEL_CURRENT)*CURRENT_MACRO);  //�Ŵ�11��
        Display_WriteValue(0,value,0);
        break;
      case  CURRENT_TEMPERATURE:  //�¶���ʾ��Χ -99�� - 99.9�棨��ʾ����-99�� - -10�� ��1�澫�ȣ�-9.9�� - 99.9����0.1�澫�ȣ�
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

