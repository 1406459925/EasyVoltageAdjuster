/********************************************
�ļ����������̹�����
���ߣ�MAGNIN
��־��
2020/06/16 MAGNIN �����ļ�

********************************************/
#include "share.h"

void Delay_Us(uint16_t us)
{
  volatile uint8_t i;
  while(us--)
  {
     i ++;
  }
}
void Delay_Ms(uint16_t ms)
{
  volatile uint8_t i;
  while(ms--)
  {
    for(i=0;i<250;++i);
    for(i=0;i<250;++i);
    for(i=0;i<250;++i);
    for(i=0;i<250;++i);
  }
}
