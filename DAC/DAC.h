#ifndef _DAC_H
#define _DAC_H
#include <stm32f4xx.h>

#define DAC_port	GPIOA
#define DAC_pin		5

void DAC_Initalise(void);
void DAC_Output(unsigned short d);
void DAC_SineWave(void);
void DAC_SquareWave(void);

#endif
