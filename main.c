#include "stm32f30x_misc.h"
#include "stm32f30x_tim.h"
#include <stm32f30x_exti.h>
#include <stm32f30x_syscfg.h>

#include "gpio.h"
#include "dac.h"
#include "adc.h"
#include "dma.h"

#include "math.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "croutine.h"
#include "queue.h"
#include "semphr.h"


xTimerHandle RTOS_tim_1;

int random32bit;
volatile int incr_x = 100;
volatile int incr_y = 100;
uint16_t pause_flag = 0;


void TimerFunction(xTimerHandle RTOS_tim)
{

	//Ball start position
	uint16_t ball_x_start[25]=
	{
			2000,2020,2040,2060,2080,
			2080,2060,2040,2020,2000,
			2000,2020,2040,2060,2080,
			2080,2060,2040,2020,2000,
			2000,2020,2040,2060,2080,
	};

	uint16_t ball_y_start[25]=
	{
			2000,2000,2000,2000,2000,
			2020,2020,2020,2020,2020,
			2040,2040,2040,2040,2040,
			2060,2060,2060,2060,2060,
			2080,2080,2080,2080,2080,
	};

	//Recalculation ball position
	int i = 0;
	for (i; i<=24; i++)
	{
		int new_x = arr_x[44+i] + incr_x;
		int new_y = arr_y[44+i] + incr_y;

		if (new_x <= 0) new_x = 0;
		else if (new_x >= 4000) new_x = 4000;

		if (new_y <= 0) new_y = 0;
		else if (new_y >= 4000) new_y = 4000;

		arr_x[44+i] = new_x;
		arr_y[44+i] = new_y;
	}

	/* Right wall impact*/
	if (arr_x[64] >= 4000)
	{
		if ((arr_y[54] > arr_y[17]-200)&(arr_y[54] < arr_y[25]+200))
		{
			random32bit = random() % 50 - 25;
			if (incr_x >= 0)
			{
				incr_x = -(100 + random32bit);
			}
			else if (incr_x < 0)
			{
				incr_x = (100 + random32bit);
			}

			if (incr_y <= 0)
			{
				incr_y = -sqrt(20000 - (incr_x * incr_x));
			}
			else
			{
				incr_y = sqrt(20000 - (incr_x * incr_x));
			}
		}
		else
		{
			xTimerStopFromISR(RTOS_tim_1, 0);
			vTaskDelay(1000);
			// Ball start position
			for (i = 0; i<=24; i++)
			{
				arr_x[44+i] =  ball_x_start[i];
				arr_y[44+i] =  ball_y_start[i];
			}
			// Start speeds
			random32bit = srandom(ADC_Result[2]) % 50 - 25;
			incr_y = random32bit;
			random32bit = random() % 50 - 25;
			if (random32bit > 0)
			{
				incr_x = -sqrt(1250 - (incr_y * incr_y));
			}
			else
			{
				incr_x = sqrt(1250 - (incr_y * incr_y));
			}
			xTimerStartFromISR(RTOS_tim_1, 0);
		}
	}
	/* Left wall impact*/
	if (arr_x[44] <= 0)

	{
		if ((arr_y[54] > arr_y[103]-200)&(arr_y[54] < arr_y[112]+200))
		{
			random32bit = random() % 50 - 25;

			if (incr_x >= 0)
			{
				incr_x = -(100 + random32bit);
			}
			else if (incr_x < 0)
			{
				incr_x = (100 + random32bit);
			}

			if (incr_y <= 0)
			{
				incr_y = -sqrt(20000 - (incr_x * incr_x));
			}
			else
			{
				incr_y = sqrt(20000 - (incr_x * incr_x));
			}
		}
		else
		{
			xTimerStopFromISR(RTOS_tim_1, 0);
			vTaskDelay(1000);
			// Ball start position
			for (i=0; i<=24; i++)
			{
				arr_x[44+i] =  ball_x_start[i];
				arr_y[44+i] =  ball_y_start[i];
			}
			// Start speeds
			random32bit = srandom(ADC_Result[2]) % 50 - 25;
			incr_y = random32bit;
			random32bit = random() % 50 - 25;
			if (random32bit > 0)
			{
				incr_x = -sqrt(1250 - (incr_y * incr_y));
			}
			else
			{
				incr_x = sqrt(1250 - (incr_y * incr_y));
			}
			xTimerStartFromISR(RTOS_tim_1, 0);
		}
	}


	if ((arr_y[64] >= 4000)|(arr_y[44] <= 0))
	{

		random32bit = random() % 50 - 25;

		if (incr_y >= 0)
		{
			incr_y = -(100 + random32bit);
		}
		else if (incr_y < 0)
		{
			incr_y = (100 + random32bit);
		}

		if (incr_x <= 0)
		{
			incr_x = -sqrt(20000 - (incr_y * incr_y));
		}
		else
		{
			incr_x = sqrt(20000 - (incr_y * incr_y));
		}
	}
}

void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 900;
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler()
{
	if (pause_flag == 0)
	{
		xTimerStartFromISR(RTOS_tim_1, 0);
		pause_flag = 1;
	}
	else if (pause_flag == 1)
	{
		xTimerStopFromISR(RTOS_tim_1, 0);
		pause_flag = 0;
	}
	EXTI_ClearFlag(EXTI_Line0);
}


void POT1_Task(void *pvParameters){
	while(1)
	{
		uint16_t racket[27] = {
				0,100,200,300,400,500,600,700,800,
				800,700,600,500,400,300,200,100,0,
				0,100,200,300,400,500,600,700,800,
		};
		uint8_t j = 0;
		uint16_t prom_res_1 = ADC_Result[0]*3;

		if(prom_res_1>=3300){
			prom_res_1 = 3300;
		}
		for(j; j<27; j++){
			arr_y[j+17] = prom_res_1+racket[j];
		}
		j = 0;
	IWDG_ReloadCounter();
	}
}

void POT2_Task(void *pvParameters){
	while(1)
	{
		uint16_t racket[27] = {
				0,100,200,300,400,500,600,700,800,
				800,700,600,500,400,300,200,100,0,
				0,100,200,300,400,500,600,700,800,
		};
		uint8_t k = 0;
		uint16_t prom_res_2 = ADC_Result[1]*3;
		for(k; k<27; k++){
			if(prom_res_2>=3300){
				prom_res_2 = 3300;
			}
			arr_y[k+86] = prom_res_2+racket[k];
		}
		k = 0;
	}
}

int main(void)
{
	Clock_Config();
	GPIO_Config();
	DMA_Config();
	DAC_Config();
	ADC_Config();
	TIM_Config();
	NVIC_Config();

	RTOS_tim_1 = xTimerCreate("TIM", pdMS_TO_TICKS(17), pdTRUE,  (void*)0, TimerFunction);
	xTaskCreate(POT1_Task,(signed char *)"POT1", configMINIMAL_STACK_SIZE, NULL, 0, (xTaskHandle * ) NULL);
	xTaskCreate(POT2_Task,(signed char *)"POT2", configMINIMAL_STACK_SIZE, NULL, 0, (xTaskHandle * ) NULL);

	//Initial state setting
	random32bit = srandom(ADC_Result[2]) % 50 - 25;
	incr_y = random32bit;
	random32bit = random() % 50 - 25;
	if (random32bit > 0)
	{
		incr_x = -sqrt(1250 - (incr_y * incr_y));

	}
	else
	{
		incr_x = sqrt(1250 - (incr_y * incr_y));
	}

	xTimerStop(RTOS_tim_1, 0);
	IWDG_Config();

	vTaskStartScheduler();

    while(1){}
}


// for FreeRTOS
void vApplicationIdleHook ( void ){}
void vApplicationMallocFailedHook ( void ){for ( ;; );}
void vApplicationStackOverflowHook ( xTaskHandle pxTask, char
*pcTaskName ){
( void ) pcTaskName;
( void ) pxTask;
for ( ;; );}
void vApplicationTickHook ( void ){}
