/********************************************
文件描述：数据类型定义
作者：MAGNIN
日志：
2020/03/16 MAGNIN 建立文件

********************************************/
#ifndef __TYPE_H
#define __TYPE_H

#define SET_BIT(x,y)  (x |= 1<<y)
#define CLR_BIT(x,y)  (x &= ~(1<<y))

typedef signed   char int8_t;
typedef unsigned char uint8_t;

typedef signed  short int int16_t;
typedef unsigned short int uint16_t;

typedef signed long int32_t;
typedef unsigned long uint32_t;


#endif
