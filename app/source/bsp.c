#include "include.h"

static void RCC_Configuration(void);




void BSP_Init(void)
{
	RCC_Configuration();
  Rs485_Configuration();
  USART3_Config(9600);
  USART1_Config(115200);
  timer_Init();
}




static void RCC_Configuration(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
	| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
}



INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    return ((INT32U)rcc_clocks.HCLK_Frequency);
}

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;
    freq = BSP_CPU_ClkFreq();
    return (freq);
}


