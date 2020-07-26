/********************************************
�ļ�������ADC�ɼ�
���ߣ�MAGNIN
��־��
2020/06/17 MAGNIN �����ļ�

********************************************/
#include <iostm8s003f3.h>
#include "adc.h"
#include "share.h"

#define ADC_OPEN SET_BIT(ADC_CR1,0)
#define ADC_OFF  CLR_BIT(ADC_CR1,0)

#define ADC_MAX_CHANNEL   5
#define ADC_VALUE_CACHE   10

static struct{
  uint8_t pos;
  uint16_t value[ADC_VALUE_CACHE];
}adcData[ADC_MAX_CHANNEL] = {0};


void ADC_Init(void)
{
  ADC_TDRL = 0x01;
  ADC_CR1 =  0x70; //18��Ƶ������ת������ֹADCת��
  ADC_CR2 |= 0x08; //��ֹɨ�裬�Ҷ��룬�ȶ��ߣ������7
  ADC_CR3 =  0x00;  //ʹ�����ݻ���
}
void ADC_INxOpen(uint8_t in)
{
  ADC_OFF;
  ADC_CSR = in;
  ADC_OPEN;
  Delay_Us(10);
  ADC_OPEN;
}

void ADC_INxConversion(uint8_t in)
{
  uint8_t l;
  if(in > ADC_MAX_CHANNEL)  return;
  while((ADC_CSR & 0x80)!=0x80); // �ȴ�ת������
  
  l = ADC_DRL;
  adcData[in].value[adcData[in].pos] = ADC_DRH;
  adcData[in].value[adcData[in].pos] <<= 8; 
  adcData[in].value[adcData[in].pos] |= l;
  ADC_CSR&=(~0x80);
  
  adcData[in].pos ++;
  if(adcData[in].pos == ADC_VALUE_CACHE)  adcData[in].pos = 0;
}

uint16_t ADC_GetValue(uint8_t in)
{
  uint16_t v=0;
  if(in > ADC_MAX_CHANNEL)  return 0;
  for(uint8_t i = 0;i < ADC_VALUE_CACHE;++i)
  {
    v += adcData[in].value[i];
  }
  return v/ADC_VALUE_CACHE;
}

