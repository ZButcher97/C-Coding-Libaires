#include "DAC.h"

void DAC_Initalise(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;			//DAC port clock enable
	DAC_port->MODER|=(3u<<(2*DAC_pin));			//DAC output pin set as anaglogue
	
	RCC->APB1ENR|=RCC_APB1ENR_DACEN;				//DAC clock enable
	DAC->CR|=DAC_CR_EN2;										//DAC 2 enabled
}

void DAC_Output(unsigned short d)
{
	DAC->DHR12R2=d;			//write data byte to DAC 2 output register pg.441
}

void DAC_SineWave(void)
{
	
}

void DAC_SquareWave(void)
{
	
}

