/********************************************
文件描述：菜单显示
作者：MAGNIN
日志：
2020/06/23 MAGNIN 建立文件

********************************************/
#include <iostm8s003f3.h>
#include <stdio.h>
#include "display.h"
#include "menu.h"
#include "type.h"
#include "share.h"
#include "controller.h"
#include "config.h"

#define ARRY_SIZE(a)    (sizeof(a)/sizeof(a[0]))

typedef enum{
  MENU_NONE,
  MENU_VOLTAGE,
  MENU_CURRENT,
  MENU_MODE,
}TypeEnumMenu;

struct StructMenu{
  TypeEnumMenu  menu;
  const char    chr;
  uint8_t       value;
  const uint8_t valueL;
  const uint8_t valueH;
  uint8_t       dot;
  uint8_t       step;
  void (*OK_Function)(uint16_t value);
};
static enum EnumPid mode;    //输出模式控制
//设置电压数值
static void Menu_VoltageOk(uint16_t value)
{
  
  Config_Write(CONFIG_VOLATGE,value);
  PID_SetVoltage(value);
}
//设置电流数值
static void Menu_CurrentOk(uint16_t value)
{
  
  Config_Write(CONFIG_CURRENT,value);
  PID_SetCurrent(value);
}
//设置模式
static void Menu_ModeOk(uint16_t value)
{
  mode = (enum EnumPid)value;
  Config_Write(CONFIG_MODE,value);
  PID_SetMode(mode);
  if(value == PID_VOLTAGE)  Display_WriteDot(0,0);
  else                      Display_WriteDot(0,1);
}

static struct StructMenu systemMenu[] = {
  {MENU_VOLTAGE,  'V',  30, 30, 120 ,  1,   1,  Menu_VoltageOk},
  {MENU_CURRENT,  'A',  1,  1,  99,    2,   2,  Menu_CurrentOk},
  {MENU_MODE,     'C',  1,  1,  2,     0,   1,  Menu_ModeOk}
};

void Menu_Init(void)
{
  Config_Init();
  systemMenu[0].value = Config_Read(CONFIG_VOLATGE);
  systemMenu[1].value = Config_Read(CONFIG_CURRENT);
  systemMenu[2].value = Config_Read(CONFIG_MODE);
  
  PID_SetVoltage(systemMenu[0].value);
  PID_SetCurrent(systemMenu[1].value);
  PID_SetMode((enum EnumPid)systemMenu[2].value);
  mode  = PID_GetMode();    //输出模式控制
}

//菜单
enum EnumCurrentPage Menu(TypeEnumKey key)
{
  static enum EnumPid power               = PID_VOLTAGE;    //输出电源控制
  static TypeEnumMenu nowMenu             = MENU_NONE;
  static enum EnumDisplay displayStatus   = OPEN_DISPALY;
  static enum EnumCurrentPage currentPage = CURRENT_VOLTAGE;
  
  if(mode == PID_VOLTAGE)  Display_WriteDot(0,0); //CV
  else                     Display_WriteDot(0,1); //CC
  
  if(nowMenu == MENU_NONE)   //常规显示
  {
    switch(key)
    {
    case KEY_UP:
      if(currentPage == CURRENT_TEMPERATURE)  currentPage = CURRENT_VOLTAGE;
      else currentPage ++;
      break;
    case KEY_DOWN:  
      if(currentPage == CURRENT_VOLTAGE)      currentPage = CURRENT_TEMPERATURE;
      else currentPage --;
      break;
    case KEY_SET:     //设置
      if(power != PID_OFF && displayStatus == OPEN_DISPALY)
      {
        Display_BlinkSeg(3);
        currentPage =CURRENT_MENU;
        nowMenu = systemMenu[mode == PID_VOLTAGE?0:1].menu;
      }
      break;
    case KEY_CHANGE:
      if(displayStatus == OPEN_DISPALY) displayStatus = OFF_DISPLAY;
      else                              displayStatus = OPEN_DISPALY;
      
      Display_Clear(CLEAR_SEG_1|CLEAR_SEG_2|CLEAR_SEG_3);
      Display_Control(displayStatus); 
      break;
    case KEY_POWER:   //关闭显示
      if(power == PID_OFF)
      {
        power = mode;
        PID_SetMode(power);
        displayStatus = OPEN_DISPALY;
      }
      else
      {
        power = PID_OFF;
        PID_SetMode(power);
        displayStatus = OFF_DISPLAY;     
      }
      Display_Clear(CLEAR_SEG_1|CLEAR_SEG_2|CLEAR_SEG_3);
      Display_Control(displayStatus); 
      break;
    default:
      nowMenu = MENU_NONE;
      break;
    }
  }
  else  //菜单显示
  {
    for(uint8_t i = 0;i < ARRY_SIZE(systemMenu);++ i )
    {
      if(systemMenu[i].menu == nowMenu && displayStatus == OPEN_DISPALY)
      {
        Display_WriteChar(3,systemMenu[i].chr);
        if(nowMenu == MENU_MODE) 
        {
          Display_WriteDot(1,0);
          Display_WriteDot(2,0);
          Display_WriteChar(2,'C');
          
          if(systemMenu[i].value == PID_VOLTAGE)  Display_WriteChar(1,'V');
          else                                    Display_WriteChar(1,'C');
        }
        else
        {
          if(systemMenu[i].value >= 100)  Display_WriteValue(1,systemMenu[i].value/10,systemMenu[i].dot - 1);
          else                            Display_WriteValue(1,systemMenu[i].value,systemMenu[i].dot);
        }
         switch(key)
        {
        case KEY_CHANGE:
          if(nowMenu == systemMenu[ARRY_SIZE(systemMenu) - 1].menu)  nowMenu = systemMenu[0].menu;
          else nowMenu ++;
          break;
        case KEY_UP:
          if(systemMenu[i].value >= systemMenu[i].valueH) systemMenu[i].value = systemMenu[i].valueL;
          else 
          {
            if(systemMenu[i].value >= 100) systemMenu[i].value += 10;
            else systemMenu[i].value += systemMenu[i].step;
          }
          break;
        case KEY_DOWN:
          if(systemMenu[i].value <= systemMenu[i].valueL) systemMenu[i].value = systemMenu[i].valueH;
          else 
          {
            if(systemMenu[i].value > 100) systemMenu[i].value -= 10;
            else systemMenu[i].value -= systemMenu[i].step;
          } 
          break; 
        case KEY_SET:
          Adress_Clear();
          systemMenu[i].OK_Function(systemMenu[i].value);
          Display_Clear(CLEAR_SEG_3);
          Display_BlinkSeg(0);
          Delay_Ms(100);
          nowMenu = MENU_NONE;
          currentPage = (mode == PID_VOLTAGE) ? CURRENT_VOLTAGE : CURRENT_CURRENT;  //主页显示
          break;
        default:
          break;
        }
        break ;
      }
    }
  }
  return currentPage;
}



