#include "clock.h"

void Clock_Config()
{
	RCC_DeInit();
	RCC_HSICmd(DISABLE);
	RCC_HSEConfig(RCC_HSE_Bypass);
	//main freq is 72 MHz
	RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
	//AHB
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//APB1
	RCC_PCLK1Config(RCC_HCLK_Div2);
	//APB2
	RCC_PCLK2Config(RCC_HCLK_Div2);
	//I2S source
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /// Wait till PLL is ready
	//PLLCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
}

void IWDG_Config()
{

  /* Set counter reload value to obtain 250ms IWDG TimeOut.
	 Counter Reload Value = 250ms * IWDG counter clock period
						  = 250ms * (LSI/32)
						  = 0.25s * (LsiFreq/32)
						  = LsiFreq / (32 * 4)
						  = LsiFreq / 128
   */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(LSI_FREQ/128);
	IWDG_ReloadCounter();
	IWDG_Enable();

}


