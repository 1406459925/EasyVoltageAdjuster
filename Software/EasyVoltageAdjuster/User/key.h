#ifndef __KEY_H
#define __KEY_H

#define KEY_ELIMINATION     100 //°´¼üÏû¶¶

typedef enum{
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_CHANGE,
  KEY_SET,
  KEY_POWER,
}TypeEnumKey;


void KEY_Init(void);
TypeEnumKey KEY_Scan(void);

#endif