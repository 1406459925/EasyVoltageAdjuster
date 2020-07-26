#ifndef TEMP_H
#define TEMP_H
#include "type.h"

#define SUPPLY_VOLTAGE                  (3.3)   //电源电压（V）
#define RESISTER_VALUE                  10000u  //分压电阻（欧姆）
#define TEMPERATURE_CALIBRATION         (-0.0)  //温度校准，按照标准温度进行校正

int16_t Calculate_TemperatureValue(uint16_t value);

#endif