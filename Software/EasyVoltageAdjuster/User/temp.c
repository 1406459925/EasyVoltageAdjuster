/********************************************
文件描述：NTC温度传感器
作者：MAGNIN
日志：
2020/07/21 MAGNIN 建立文件

********************************************/
#include "temp.h"
#include "math.h"

#define NTC_RP    10000u           //T2常温下标称阻值
#define NTC_T2    (273.15 + 25)   //T2温度值（开尔文温度）
#define NTC_BX    (3950.0)        //NTC B值
#define NTC_KA    (273.15)        //绝对零度
//输入电压值V*1000
int16_t Calculate_TemperatureValue(uint16_t value)
{
  float tv = (float)value/1000;
  float rt = tv*RESISTER_VALUE/(SUPPLY_VOLTAGE - tv);
  float temp = 1/(1 / NTC_T2 + log(rt / NTC_RP) / NTC_BX) - NTC_KA + TEMPERATURE_CALIBRATION;
  temp *= 10;
  return (int16_t)temp;
}