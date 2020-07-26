/********************************************
�ļ��������洢��
���ߣ�MAGNIN
��־��
2020/04/06 MAGNIN �����ļ�

********************************************/
#include "eeprom.h"
#include <iostm8s003f3.h>

#define MASS_KEY1     0xAE
#define MASS_KEY2     0x56

//EEPROM��ʼ��
void EEPROM_Init(void)
{
  FLASH_CR1 = 0x00;
  FLASH_CR2 = 0x00;
  FLASH_NCR2 = 0xFF;
  FLASH_IAPSR &= 0xF7;   //д����ʹ��
  (void)FLASH_IAPSR;     //�����־λ
}
//EEPROM����
void EEPROM_Lock(void)
{
  FLASH_IAPSR &=  0xF7;
}
//EERPROM����
void EEPROM_Unlock(void)
{
  FLASH_DUKR = MASS_KEY1;
  FLASH_DUKR = MASS_KEY2;
}

//��ȡ����
void EEPROM_ReadBuffer(uint32_t address,uint8_t number,uint8_t *buffer)
{
  uint32_t addr = address;
  for(uint8_t count = 0;count < number;++count)
  {
    buffer[count] = (*(__near uint8_t *) (uint16_t)addr); 
    addr ++;
  }
}
//����
__ramfunc void EEPROM_BlockProgram(uint8_t bnum,uint8_t *buffer)
{
  uint32_t address = FLASH_DATA_START_PHYSICAL_ADDRESS + bnum * FLASH_BLOCK_SIZE;
  FLASH_CR2 |= 0x01;  //�رտ��ٿ���
  FLASH_NCR2 &= 0xEF; //��׼����
  for(uint8_t count = 0;count < FLASH_BLOCK_SIZE;++count)
  {
    *((__near uint8_t*) (uint16_t)address + count) = ((uint8_t)(buffer[count]));
  }
}



