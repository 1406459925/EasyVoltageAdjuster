/********************************************
�ļ��������ϵ��������
���ߣ�MAGNIN
��־��
2020/06/06 MAGNIN �����ļ�

********************************************/
#include "config.h"
#include "eeprom.h"

#define CONFIG_FLAG   0x4D42

static struct StructConfig{
  uint32_t flag;      //��־λ
  uint32_t mode;      //���ģʽ
  uint32_t voltage;   //���õ�ѹֵ
  uint32_t current;   //���õ���ֵ
  
  uint8_t reserve[FLASH_BLOCK_SIZE - sizeof(uint32_t )*4 ]; //��������
}config;

//���ó�ʼ��
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
//��ȡ����
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
//д������
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