#include <stm32f4xx.h>
/* 
		This program has been written to set up the inturrupts. 
		
		When included in a program, remember to include the initalise function to set up the registers/ports.
		This program was designed specificlly for the STM32-F429Zi
*/



//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


/*
ARR=(F_APB/(PSC+1))·T_delay

ARR is the Auto reload register. This is the register we will use to set the times duration. This is done by adjusting the Tdelay in the formula. 
Fapb is determined by Fcy/2. This is the core clock /2 = 180MHz / 2 = 2 90MHz. 
We set (PSC + 1) = 256. Then 90MHz / 256 = 35156. This would give us a delay of 100ms. 35156 is our ARR value. 

*/

/*
If a different inturrupt is desired; the clock enable, DIER, PSC, ARR, CNT and CR1 must be changed simply by replacing TIM2 with the desired Inturrupt. however, 
NVIC->ISER[0]|=(1u<<xx) where xx is the position of the Inturrupt wanted. This can be found in RM0090 pg. 373
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void INT_Initalise_TIM2(void) //This function is used to initalise the registers and ports for the inturrputs.
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

TIM2->DIER |=TIM_DIER_UIE;


TIM2->PSC=256-1;
TIM2->ARR=35156;
TIM2->CNT=0;
NVIC->ISER[0]|=(1u<<28); //this is the global interrupt enable. 28 is timer 2 and 29 is timer 3. found in RM0090 pg. 373
TIM2->CR1 |=TIM_CR1_CEN;

}


/*

Below is the function to be used in the project Main.c and is the hadler used when TIM2 inturrupt activates.

void TIM2_IRQHandler(void)
{

}


*/

