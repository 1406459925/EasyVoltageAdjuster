/********************************************
文件描述：断电参数配置
作者：MAGNIN
日志：
2020/06/06 MAGNIN 建立文件

********************************************/
#include "config.h"
#include "eeprom.h"

#define CONFIG_FLAG   0x4D42

static struct StructConfig{
  uint32_t flag;      //标志位
  uint32_t mode;      //输出模式
  uint32_t voltage;   //设置电压值
  uint32_t current;   //设置电流值
  
  uint8_t reserve[FLASH_BLOCK_SIZE - sizeof(uint32_t )*4 ]; //保留区域
}config;

//配置初始化
void Config_Init(void)
{
  EEPROM_Init();
  EEPROM_ReadBuffer(FLASH_DATA_START_PHYSICAL_ADDRESS,FLASH_BLOCK_SIZE,(uint8_t *)&config);
  if(config.flag != CONFIG_FLAG)
  {
    config.flag    = CONFIG_FLAG;
    config.mode    = CONFIG_DEFAULT_MODE;
    config.voltage = CONFIG_DEFAULT_VOLTAGE;
    config.current = CONFIG_DEFAULT_CURRENT;
    EEPROM_Unlock();
    EEPROM_BlockProgram(0,(uint8_t *)&config);
    EEPROM_Lock();
  }
}  
//读取配置
uint32_t Config_Read(enum EnumConfig pos)
{
  uint32_t value;
  switch(pos)
  {
  case CONFIG_MODE:
    value = config.mode;
    break;
  case CONFIG_VOLATGE:
    value = config.voltage;
    break;
  case CONFIG_CURRENT:
    value = config.current;
    break;
  default:break;
  }
  return value;
}
//写入配置
void Config_Write(enum EnumConfig pos,uint32_t value)
{
  switch(pos)
  {
  case CONFIG_MODE:
    config.mode = value;
    break;
  case CONFIG_VOLATGE:
    config.voltage = value;
    break;
  case CONFIG_CURRENT:
    config.current  = value;
    break;
  default:break;
  }
  EEPROM_Unlock();
  EEPROM_BlockProgram(0,(uint8_t *)&config);
  EEPROM_Lock();
}