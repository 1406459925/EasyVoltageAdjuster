#ifndef TEMP_H
#define TEMP_H
#include "type.h"

#define SUPPLY_VOLTAGE                  (3.3)   //��Դ��ѹ��V��
#define RESISTER_VALUE                  10000u  //��ѹ���裨ŷķ��
#define TEMPERATURE_CALIBRATION         (-0.0)  //�¶�У׼�����ձ�׼�¶Ƚ���У��

int16_t Calculate_TemperatureValue(uint16_t value);

#endif