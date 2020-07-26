/********************************************
�ļ�������74HC595����
���ߣ�MAGNIN
��־��
2020/06/16 MAGNIN �����ļ�

********************************************/
#include <iostm8s003f3.h>
#include "74hc595.h"
#include "type.h"
//74HC595��ʼ������
void HC595_Init(void)
{
  SET_BIT(PC_DDR,5);  //����Ϊ���
  SET_BIT(PC_CR1,5);  //����Ϊ�������
  SET_BIT(PC_CR2,5);  //�����ٶ�Ϊ10M
  
  SET_BIT(PC_DDR,6);  //����Ϊ���
  SET_BIT(PC_CR1,6);  //����Ϊ�������
  SET_BIT(PC_CR2,6);  //�����ٶ�Ϊ10M
  
  SET_BIT(PC_DDR,7);  //����Ϊ���
  SET_BIT(PC_CR1,7);  //����Ϊ�������
  SET_BIT(PC_CR2,7);  //�����ٶ�Ϊ10M
  
  HC595_SER_CLR;
  HC595_RCLK_CLR;
  HC595_SRCLK_CLR;
}
//74HC595д��һ���ֽ�
void HC595_WriteByte(uint8_t byte)
{
  byte = ~byte;
  for(uint8_t i = 0;i < 8;++ i )
  {
    if(byte & 0x80) HC595_SER_SET;
    else  HC595_SER_CLR;
    
    HC595_SRCLK_SET;
    Delay_Us(1);
    
    HC595_SER_CLR;
    HC595_SRCLK_CLR;
    
    HC595_RCLK_SET;
    
    Delay_Us(1);
  
    HC595_RCLK_CLR;
    
    byte <<=1;
  }
}
