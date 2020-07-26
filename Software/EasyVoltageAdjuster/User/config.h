#ifndef CONFIG_H
#define CONFIG_H
#include "type.h"

//��������
enum EnumConfig{
  CONFIG_MODE,
  CONFIG_VOLATGE,
  CONFIG_CURRENT
};
//Ĭ�ϲ������ã��豸����ʹ��ʱ�Ĳ���
#define CONFIG_DEFAULT_MODE         1U    //Ĭ��ģʽ
#define CONFIG_DEFAULT_VOLTAGE      33U   //Ĭ�ϵ�ѹ
#define CONFIG_DEFAULT_CURRENT      10U   //Ĭ�ϵ���


void Config_Init(void);
uint32_t Config_Read(enum EnumConfig pos);
void Config_Write(enum EnumConfig pos,uint32_t value);
#endif
