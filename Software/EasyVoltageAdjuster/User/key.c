/********************************************
�ļ����������밴��
���ߣ�MAGNIN
��־��
2020/06/23 MAGNIN �����ļ�
2020/06/23 MAGNIN ��ɰ���������д
********************************************/
#include <iostm8s003f3.h>
#include "key.h"
#include "type.h"
#define  KEY_PRESS        0
#define  KEY_UP_STATUS    PD_IDR_IDR4
#define  KEY_DOWN_STATUS  PD_IDR_IDR5

void KEY_Init(void)
{
  CLR_BIT(PD_DDR,4);  //����Ϊ����
  CLR_BIT(PD_CR1,4);  //����Ϊ��������ģʽ
  CLR_BIT(PD_CR2,4);  //�����ٶ�Ϊ10M
  
  CLR_BIT(PD_DDR,5);  //����Ϊ����
  CLR_BIT(PD_CR1,5);  //����Ϊ��������ģʽ
  CLR_BIT(PD_CR2,5);  //�����ٶ�Ϊ10M
  
}
//����ɨ��
TypeEnumKey KEY_Scan(void)
{
  static uint8_t timeDelay = 0;
  static TypeEnumKey key = KEY_NONE;
  switch(key)
  {
  case KEY_NONE:
    if(KEY_UP_STATUS == KEY_PRESS)    key = KEY_UP;   //up����
    else if(KEY_DOWN_STATUS == KEY_PRESS)  key = KEY_CHANGE; //down����
    else key = KEY_NONE;
    break;
  case KEY_UP:
    if(timeDelay > KEY_ELIMINATION) //��ʱ��
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
    if(timeDelay > KEY_ELIMINATION) //��ʱ��
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
    if(timeDelay > KEY_ELIMINATION) //��ʱ��
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