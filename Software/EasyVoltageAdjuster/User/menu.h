#ifndef __MENU_H
#define __MENU_H
#include "key.h"

enum EnumCurrentPage{
  CURRENT_MENU,
  CURRENT_OFF,
  CURRENT_VOLTAGE,
  CURRENT_CURRENT,
  CURRENT_TEMPERATURE,
};
void Menu_Init(void);
enum EnumCurrentPage Menu(TypeEnumKey key);

#endif 