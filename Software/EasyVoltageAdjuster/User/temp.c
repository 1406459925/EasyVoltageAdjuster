/********************************************
�ļ�������NTC�¶ȴ�����
���ߣ�MAGNIN
��־��
2020/07/21 MAGNIN �����ļ�

********************************************/
#include "temp.h"
#include "math.h"

#define NTC_RP    10000u           //T2�����±����ֵ
#define NTC_T2    (273.15 + 25)   //T2�¶�ֵ���������¶ȣ�
#define NTC_BX    (3950.0)        //NTC Bֵ
#define NTC_KA    (273.15)        //�������
//�����ѹֵV*1000
int16_t Calculate_TemperatureValue(uint16_t value)
{
  float tv = (float)value/1000;
  float rt = tv*RESISTER_VALUE/(SUPPLY_VOLTAGE - tv);
  float temp = 1/(1 / NTC_T2 + log(rt / NTC_RP) / NTC_BX) - NTC_KA + TEMPERATURE_CALIBRATION;
  temp *= 10;
  return (int16_t)temp;
}