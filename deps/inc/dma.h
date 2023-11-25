#include "stm32f30x_dma.h"
#include "adc.h"

#define ARRAY_SIZE 113

extern uint16_t arr_x[ARRAY_SIZE];
extern uint16_t arr_y[ARRAY_SIZE];
extern uint16_t ADC_Result[3];

DMA_InitTypeDef DMA_InitStructure;

void DAC_Config(void);
