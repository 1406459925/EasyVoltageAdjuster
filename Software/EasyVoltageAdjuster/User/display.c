/********************************************
�ļ��������������ʾ
���ߣ�MAGNIN
��־��
2020/03/16 MAGNIN �����ļ�
2020/03/23 MAGNIN �����������
********************************************/
#include "display.h"
#include "74hc595.h"
#include "share.h"
#include "stdio.h"
/********************************************
  A1--------------------PC3
  A2--------------------PB4
  A3--------------------PB5
********************************************/

//-------------------------- 0    1    2     3    4    5    6    7    8    9    A   b    C    d    E    F    V    S    -
const uint8_t ledNumber[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x3E,0x6D,0x40};

static struct{
  uint8_t enable  :1;
  uint8_t forcus  :2;
  uint8_t nowseg  :2;
  uint8_t seg[3];
  uint8_t blinktime;
  uint8_t timecnt;
}ledMemory = {0};

void Adress_Clear(void)
{
  A1_CLR; 
  A2_CLR;
  A3_CLR;
  //Delay_Us(100);
}

void Display_Init(void)
{
  SET_BIT(PC_DDR,3);  //����Ϊ���
  SET_BIT(PC_CR1,3);  //����Ϊ�������
  SET_BIT(PC_CR2,3);  //�����ٶ�Ϊ10M
  
  SET_BIT(PB_DDR,4);  //����Ϊ���
  SET_BIT(PB_CR1,4);  //����Ϊ�������
  SET_BIT(PB_CR2,4);  //�����ٶ�Ϊ10M
  
  SET_BIT(PB_DDR,5);  //����Ϊ���
  SET_BIT(PB_CR1,5);  //����Ϊ�������
  SET_BIT(PB_CR2,5);  //�����ٶ�Ϊ10M
 
  HC595_Init();
  //�ر���ʾ
  Adress_Clear();
  //������ʾ
  ledMemory.enable = OPEN_DISPALY; 
  ledMemory.nowseg = 1;
  ledMemory.forcus = 0;
  ledMemory.blinktime = DEFAULT_BLINK_TIME;
}
//����
void Display_Clear(uint8_t seg)
{
  if(seg & 0x01)  ledMemory.seg[0] = 0;
  if(seg & 0x02)  ledMemory.seg[1] = 0;
  if(seg & 0x04)  ledMemory.seg[2] = 0;
  
}
//��ʾ����
void Display_Control(enum EnumDisplay control)
{
  ledMemory.enable = control;
}
//��ʾ����
void Display_BlinkSeg(uint8_t seg)
{
  if(seg > 3) return;
  if(seg == 0) ledMemory.forcus = 0;
  else ledMemory.forcus = 4 - seg;
}
//д���
void Display_WriteDot(uint8_t dot,uint8_t reverse)
{
  if(dot >= 3) return ;
  if(reverse) ledMemory.seg[dot] |= 0x80;
  else ledMemory.seg[dot] &= 0x7F;
}
//д���ַ�
void Display_WriteChar(uint8_t seg,char chr)
{
  if(seg > 3 || seg == 0) return ;
  seg -= 1;
  ledMemory.seg[seg] &= 0x80;
  switch(chr)
  {
    case '0': 
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ledMemory.seg[seg] |= ledNumber[chr - '0']; break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      ledMemory.seg[seg] |= ledNumber[chr - 'a' + 10]; break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      ledMemory.seg[seg] |= ledNumber[chr - 'A' + 10]; break;
    case 'V':
      ledMemory.seg[seg] |= ledNumber[16]; break;
    case 'S':
    ledMemory.seg[seg] |= ledNumber[17]; break;
    case '-':
    ledMemory.seg[seg] |= ledNumber[18]; break;
    case ' ':
    ledMemory.seg[seg] |= 0; break;
  default:break;
  }
}
//д����ֵ
void Display_WriteValue(uint8_t mask,uint16_t value,uint8_t decimal)
{
  uint16_t cValue = value;
  if(value & 0x8000) //�и���
  {
    value = - (int16_t)value;
    if(value >= 10) decimal = 0;
  }
  //if(decimal > 2 || mask > 2) return; //д�볬��Χ
  //д���
  if(decimal == 1)
  {
    Display_WriteDot(decimal,1);
    Display_WriteDot(2,0);
  }
  else if(decimal == 2)
  {
    Display_WriteDot(decimal,1);
    Display_WriteDot(1,0);
  }
  else
  {
    Display_WriteDot(1,0);
    Display_WriteDot(2,0);
  }
  //д������
  for(uint8_t i = 3 -mask;i > 0;--i)
  {
    ledMemory.seg[3 -mask - i] &= 0x80; //������
    if(i == 1 && (cValue & 0x8000))  //�и���
    {
      ledMemory.seg[3 -mask - i] |= ledNumber[18];
    }
    else
    {
      ledMemory.seg[3 -mask - i] |= ledNumber[value % 10];
      value /= 10;
    }
    
  }
}
//��ʾ��ˢ�º������ڲ�������ʱ�����������ⲿ��ʱ5ms
void Display_Refresh(void)
{
   if(ledMemory.enable)
   { 
     if(ledMemory.forcus == ledMemory.nowseg)
     {
       if(++ ledMemory.timecnt < ledMemory.blinktime)
       {
        Adress_Clear();
        if(ledMemory.nowseg  == 3) ledMemory.nowseg = 1;
        else ledMemory.nowseg++;
        return ;
       }
     }
     if(ledMemory.timecnt  >= 100) ledMemory.timecnt = 0;
     switch(ledMemory.nowseg)
     {
       case 1:
         Adress_Clear();
         
         HC595_WriteByte(ledMemory.seg[2]); 
         A1_SET;
         break;
       case 2:
         Adress_Clear();
         
         HC595_WriteByte(ledMemory.seg[1]);
         A2_SET;
         break; 
       case 3:
         Adress_Clear();
         
         HC595_WriteByte(ledMemory.seg[0]);
         A3_SET;
         break;
       default:
         break;
     }  
     if(ledMemory.nowseg  == 3) ledMemory.nowseg = 1;
     else ledMemory.nowseg++;
   }
   else Adress_Clear();
}

//��������Ч��
void Display_Logo(void)
{
  uint16_t time =0;
  Delay_Ms(100);
  while(time ++ < 1620)
  {
    if(time == 1)
    {
      Display_WriteChar(3,' '); 
      Display_WriteChar(2,' '); 
      Display_WriteChar(1,'V');
    }
    else if(time == 200)
    {
      Display_WriteChar(3,' '); 
      Display_WriteChar(2,'V');
      Display_WriteChar(1,'S'); 
    }
    else if(time == 350)
    {
      Display_WriteChar(3,'V');
      Display_WriteChar(2,'S'); 
      Display_WriteChar(1,'8'); 
      
    }
    else if(time == 2400)
    {
    Display_WriteChar(3,'S'); 
    Display_WriteChar(2,'8');
    Display_WriteChar(1,' '); 
    }
    else if(time == 2500)
    {
    Display_WriteChar(3,'8');
     Display_WriteChar(2,' '); 
     Display_WriteChar(1,' '); 
    }
    else if(time == 2600)
    {
     Display_WriteChar(3,' ');
     Display_WriteChar(2,' '); 
     Display_WriteChar(1,' '); 
    }
    Display_Refresh();
    Delay_Ms(2);
  }
}

