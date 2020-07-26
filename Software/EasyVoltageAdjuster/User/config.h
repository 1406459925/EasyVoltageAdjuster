#ifndef CONFIG_H
#define CONFIG_H
#include "type.h"

//配置类型
enum EnumConfig{
  CONFIG_MODE,
  CONFIG_VOLATGE,
  CONFIG_CURRENT
};
//默认参数配置，设备初次使用时的参数
#define CONFIG_DEFAULT_MODE         1U    //默认模式
#define CONFIG_DEFAULT_VOLTAGE      33U   //默认电压
#define CONFIG_DEFAULT_CURRENT      10U   //默认电流


void Config_Init(void);
uint32_t Config_Read(enum EnumConfig pos);
void Config_Write(enum EnumConfig pos,uint32_t value);
#endif
