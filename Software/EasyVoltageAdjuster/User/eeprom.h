#ifndef __EEPROM_H
#define __EEPROM_H
#include "type.h"

#define FLASH_BLOCK_SIZE                  ((uint8_t)64)
#define FLASH_DATA_START_PHYSICAL_ADDRESS ((uint32_t)0x004000) /*!< Data EEPROM memory: start address */

void EEPROM_Init(void);
void EEPROM_Lock(void);
void EEPROM_Unlock(void);
void EEPROM_ReadBuffer(uint32_t address,uint8_t number,uint8_t *buffer);
__ramfunc void EEPROM_BlockProgram(uint8_t bnum,uint8_t *buffer);
#endif
