/********************************************
文件描述：74HC595驱动
作者：MAGNIN
日志：
2020/06/16 MAGNIN 建立文件

********************************************/
#include <iostm8s003f3.h>
#include "74hc595.h"
#include "type.h"
//74HC595初始化函数
void HC595_Init(void)
{
  SET_BIT(PC_DDR,5);  //设置为输出
  SET_BIT(PC_CR1,5);  //设置为推挽输出
  SET_BIT(PC_CR2,5);  //设置速度为10M
  
  SET_BIT(PC_DDR,6);  //设置为输出
  SET_BIT(PC_CR1,6);  //设置为推挽输出
  SET_BIT(PC_CR2,6);  //设置速度为10M
  
  SET_BIT(PC_DDR,7);  //设置为输出
  SET_BIT(PC_CR1,7);  //设置为推挽输出
  SET_BIT(PC_CR2,7);  //设置速度为10M
  
  HC595_SER_CLR;
  HC595_RCLK_CLR;
  HC595_SRCLK_CLR;
}
//74HC595写入一个字节
void HC595_WriteByte(uint8_t byte)
{
  byte = ~byte;
  for(uint8_t i = 0;i < 8;++ i )
  {
    if(byte & 0x80) HC595_SER_SET;
    else  HC595_SER_CLR;
    
    HC595_SRCLK_SET;
    Delay_Us(1);
    
    HC595_SER_CLR;
    HC595_SRCLK_CLR;
    
    HC595_RCLK_SET;
    
    Delay_Us(1);
  
    HC595_RCLK_CLR;
    
    byte <<=1;
  }
}
