/********************************************
�ļ��������������
���ߣ�MAGNIN
��־��
2020/06/28 MAGNIN �����ļ�
2020/06/28 MAGNIN �汾V1.0
********************************************/
#include <iostm8s003f3.h>
#include "infrared.h"
#include "tim.h"

#define IRA_PIN            PD_IDR_IDR6

#define INFRA_REV_IDLE     0
#define INFRA_REV_START    1
#define INFRA_REV_OK       2

static struct{
  uint8_t status   :2;    //�Ƿ�������
  uint8_t position :6;
  uint8_t data[4];
}infrared = {0};

void Infrared_Init(void)
{
  CLR_BIT(PD_DDR,6);      //�������ݷ���Ĵ��� 1Ϊ�����0Ϊ����
  CLR_BIT(PD_CR1,6);      //��������
  SET_BIT(PD_CR2,6);      //�����ж�
  
  EXTI_CR1 &= (uint8_t)(~MASK_EXTI_CR1_PDIS);
  EXTI_CR1 |= (uint8_t)((uint8_t)(0x02) << 6);/*����Ϊ�½��ش����ж�*/
}
//��ȡ��������ֵ
uint8_t Infrared_GetValue(void)
{
  if(infrared.status == INFRA_REV_OK)
  {
    infrared.status = INFRA_REV_IDLE;
    return infrared.data[2]; 
  }
  return 0;
}

#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
  if(!IRA_PIN)
  {
    uint8_t time = Get_TimeCount();
    switch(time)
    {
    case 0:
    case 1:
    case 2:
    case 3:
      if(infrared.status == INFRA_REV_START)    //0
      {
        CLR_BIT(infrared.data[infrared.position / 8] ,infrared.position % 8);
        infrared.position ++;
      }
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      if(infrared.status == INFRA_REV_START)    //1
      {
        SET_BIT(infrared.data[infrared.position / 8] ,infrared.position % 8);
        infrared.position ++; 
      }
      break;
    case 20:
    case 21:
    case 22:
      if( INFRARED_KEY_UP  == infrared.data[2] || INFRARED_KEY_DOWN == infrared.data[2])  infrared.status = INFRA_REV_OK;  //���°���֧�ֳ���
      break;
    default:
      infrared.position = 0;
      if(infrared.status == INFRA_REV_IDLE) infrared.status = INFRA_REV_START;
      break;
    }
    if(infrared.position >= 32)
    {
      infrared.position = 0;
      if(infrared.data[2] == (~infrared.data[3]&0xff))
      {
        infrared.status = INFRA_REV_OK;
      }
      else infrared.status = INFRA_REV_IDLE;
    }
    Set_TimeCount(0);
  }
}