#ifndef _ADC_H
#define _ADC_H
#include <stm32f4xx.h>

#define ADC_input_port		GPIOC
#define ADC_input_pin			0
#define ADC_Channel				10

void ADC_Initalise(void);
unsigned short ADC_Read(void);

#endif 
