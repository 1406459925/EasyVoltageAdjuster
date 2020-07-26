/********************************************
文件描述：存储器
作者：MAGNIN
日志：
2020/04/06 MAGNIN 建立文件

********************************************/
#include "eeprom.h"
#include <iostm8s003f3.h>

#define MASS_KEY1     0xAE
#define MASS_KEY2     0x56

//EEPROM初始化
void EEPROM_Init(void)
{
  FLASH_CR1 = 0x00;
  FLASH_CR2 = 0x00;
  FLASH_NCR2 = 0xFF;
  FLASH_IAPSR &= 0xF7;   //写保护使能
  (void)FLASH_IAPSR;     //清除标志位
}
//EEPROM锁定
void EEPROM_Lock(void)
{
  FLASH_IAPSR &=  0xF7;
}
//EERPROM解锁
void EEPROM_Unlock(void)
{
  FLASH_DUKR = MASS_KEY1;
  FLASH_DUKR = MASS_KEY2;
}

//读取数据
void EEPROM_ReadBuffer(uint32_t address,uint8_t number,uint8_t *buffer)
{
  uint32_t addr = address;
  for(uint8_t count = 0;count < number;++count)
  {
    buffer[count] = (*(__near uint8_t *) (uint16_t)addr); 
    addr ++;
  }
}
//块编程
__ramfunc void EEPROM_BlockProgram(uint8_t bnum,uint8_t *buffer)
{
  uint32_t address = FLASH_DATA_START_PHYSICAL_ADDRESS + bnum * FLASH_BLOCK_SIZE;
  FLASH_CR2 |= 0x01;  //关闭快速块编程
  FLASH_NCR2 &= 0xEF; //标准块编程
  for(uint8_t count = 0;count < FLASH_BLOCK_SIZE;++count)
  {
    *((__near uint8_t*) (uint16_t)address + count) = ((uint8_t)(buffer[count]));
  }
}



