#include "dma.h"

uint16_t arr_x[ARRAY_SIZE] = {
		0,250,500,750,1000,1250,1500,1750,2000,2250,2500,2750,3000,3250,3500,3750,4000,
		4000,4000,4000,4000,4000,4000,4000,4000,4000,
		4000,4000,4000,4000,4000,4000,4000,4000,4000,
		4000,4000,4000,4000,4000,4000,4000,4000,4000,
		2000,2020,2040,2060,2080,
		2080,2060,2040,2020,2000,
		2000,2020,2040,2060,2080,
		2080,2060,2040,2020,2000,
		2000,2020,2040,2060,2080,
		4000,3750,3500,3250,3000,2750,2500,2250,2000,1750,1500,1250,1000,750,500,250,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,
};


uint16_t arr_y[ARRAY_SIZE] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1600,1700,1800,1900,2000,2100,2200,2300,2400,
		2400,2300,2200,2100,2000,1900,1800,1700,1600,
		1600,1700,1800,1900,2000,2100,2200,2300,2400,
		2000,2000,2000,2000,2000,
		2020,2020,2020,2020,2020,
		2040,2040,2040,2040,2040,
		2060,2060,2060,2060,2060,
		2080,2080,2080,2080,2080,
		4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,4000,
		1600,1700,1800,1900,2000,2100,2200,2300,2400,
		2400,2300,2200,2100,2000,1900,1800,1700,1600,
		1600,1700,1800,1900,2000,2100,2200,2300,2400,
};

uint16_t ADC_Result[3];
void DMA_Config(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    /* DMA2 channel3 configuration */
    DMA_DeInit(DMA2_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&arr_x;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = ARRAY_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel3, ENABLE);

    DMA_DeInit(DMA2_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R2;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&arr_y;
    DMA_InitStructure.DMA_BufferSize = ARRAY_SIZE;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel4, ENABLE);

// DMA for ADCs
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1 -> DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Result[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
