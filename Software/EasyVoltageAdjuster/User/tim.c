/********************************************
文件描述：定时器
作者：MAGNIN
日志：
2020/06/17 MAGNIN 建立文件

********************************************/
#include <iostm8s003f3.h>
#include "tim.h"
#include "controller.h"

#define TIM2_OCMODE_PWM1        0x60 
#define TIM2_OUTPUTSTATE_ENABLE 0x11
#define TIM2_OCPOLARITY_HIGH    0x00 

static uint8_t cnt;
static uint8_t timecnt;

/**************************************************************************
 * 函数名：Tim1_Time_Upmode_conf
 * 描述  ：TIM1定时器功能初始化
 * 输入  ：uint8_t TIM1_Prescaler 
           uint16_t TIM1_Period   1～65535
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用 
 *************************************************************************/
void TIM1_Init(uint16_t TIM1_Prescaler,uint16_t TIM1_Period,uint8_t TIM1_RepetitionCounter)
{
    /* Set the Autoreload value */
  TIM1_ARRH = (uint8_t)(TIM1_Period >> 8);
  TIM1_ARRL = (uint8_t)(TIM1_Period);

  /* Set the Prescaler value */
  TIM1_PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
  TIM1_PSCRL = (uint8_t)(TIM1_Prescaler);

  /* Select the Counter Mode */
  TIM1_CR1 = (uint8_t)((uint8_t)(TIM1_CR1 & (uint8_t)(~(MASK_TIM1_CR1_CMS  | MASK_TIM1_CR1_DIR)))
                         | (uint8_t)(TIM1_COUNTERMODE_UP));

  /* Set the Repetition Counter value */
  TIM1_RCR = TIM1_RepetitionCounter;
  /* Set the ARPE Bit */
  TIM1_CR1 |= MASK_TIM1_CR1_ARPE;
  /* Enable the Interrupt Upmode sources */
  TIM1_IER |= (uint8_t)TIM1_IT_UPDATE;
  /* set or Reset the CEN Bit */
  TIM1_CR1 |= MASK_TIM1_CR1_CEN;
}
/**************************************************************************
 * 函数名：TIM2_OC1Init
 * 描述  ：TIM2 第一通道PWM初始化
 * 输入  ：TIM2_OCMode   常用参数TIM2_OCMODE_PWM1:0x60 ,TIM2_OCMODE_PWM2:0x70
           TIM2_OutputState  TIM2_OUTPUTSTATE_ENABLE:0x11
           TIM2_Pulse     占空比                  
           TIM2_OCPolarity  TIM2_OCPOLARITY_HIGH:0x00 ,TIM2_OCPOLARITY_LOW:0x22
 *************************************************************************/
void TIM2_OC3Init(uint8_t TIM2_OCMode,uint8_t TIM2_OutputState,uint16_t TIM2_Pulse,uint8_t TIM2_OCPolarity )
{
    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
  TIM2_CCER2 &= (uint8_t)(~( MASK_TIM2_CCER2_CC3E | MASK_TIM2_CCER2_CC3P));
  /* Set the Output State &  Set the Output Polarity  */
  TIM2_CCER2 |= (uint8_t)((uint8_t)(TIM2_OutputState & MASK_TIM2_CCER2_CC3E ) | 
                           (uint8_t)(TIM2_OCPolarity & MASK_TIM2_CCER2_CC3P));

  /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
  TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~(uint8_t)MASK_TIM2_CCMR3_OC3M )) |
                          (uint8_t)TIM2_OCMode);
  /* Set the Pulse value */
  TIM2_CCR3H = (uint8_t)(TIM2_Pulse >> 8);
  TIM2_CCR3L = (uint8_t)(TIM2_Pulse);
  TIM2_CCMR3 |= (uint8_t)MASK_TIM2_CCMR3_OC3PE;
}
void PWM_Init(void)
{
  TIM2_PSCR =0x00;    //2^4 16分频
  /* Set the Autoreload value */
  TIM2_ARRH = (uint8_t)(ARR >> 8);
  TIM2_ARRL = (uint8_t)(ARR);
  /*TIM2 Frequency=16M/1/(319+1)=50K*/
  TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,0, TIM2_OCPOLARITY_HIGH);
  TIM2_CR1 |= (uint8_t)MASK_TIM2_CR1_CEN; //开启定时器
  
}
void PWM_SetFrequency(uint16_t TIM2_Period)
{
  TIM2_ARRH = (uint8_t)(TIM2_Period >> 8);
  TIM2_ARRL = (uint8_t)(TIM2_Period);
}

void PWM_SetDutyCycle( uint16_t TIM2_Pulse)
{
  TIM2_CCR3H = (uint8_t)(TIM2_Pulse >> 8);
  TIM2_CCR3L = (uint8_t)(TIM2_Pulse);
}
void PWM_Open(void)
{
  TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~(uint8_t)MASK_TIM2_CCMR3_OC3M ))|(uint8_t)TIM2_OCMODE_PWM1);
}
void PWM_Off(void)
{
  TIM2_CCMR3 &= (uint8_t)~0x40;
}
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
{
  if(timecnt < 255) timecnt++;
  if(++cnt > 2)
  {
    PID_Controller();
    cnt = 0;
  }
  /* Clear the IT pending Bit */
  TIM1_SR1 = (uint8_t)(~(uint8_t)TIM1_IT_UPDATE);
  
}

uint8_t Get_TimeCount(void)
{
 return timecnt;
}

void Set_TimeCount(uint8_t Count)
{
  timecnt = Count;
}