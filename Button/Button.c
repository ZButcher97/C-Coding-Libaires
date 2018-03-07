#include <stm32f4xx.h>
#include "PLL_Config.c"
#include "Button.h"	

//Github test Comment

void BUTTONT_Initalise(void)
{
	PLL_Config();
	SystemCoreClockUpdate();
		

	
	
		//ENABLE PORT
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;		//GPIO C clock enable
		
		//CONFIGURE PORT PIN FUNCTIONS
	GPIOC->MODER&=~(3u<<(2*13)); //Sets GPIOC to input mode, binery 11 in the MODER register
		
	//int ButtonRead = 0u;		//Initilises ButtonRead to 0
	//int ButtonMask = (1<<13); 		//Sets up a mask because Push button 1 (On board push button) is set to PC13
	
}	



/*
this function is used to return a 0 if the switch is not pressed, or a 1 if pressed. The input is read 5 times, if for those 5 times
the same input value is read, the functio will return the appropiate value. If (because of switch bounce) the value is not read the 
same 5 times, the function will reread the input 5 more times unitl the input is stabe and then output the appropiate value.
*/

int BUTTON_PressCheck(void)
{
	while(1){
		int Button_Bounce_Counter = 40;
		for(int i = 0; i <= 20; i++)
		{
			int ButtonRead = 0u;				//Initilises ButtonRead to 0
			ButtonRead = GPIOC->IDR & ButtonMask; 		//Masks the IDR and the mask together in the variable
			if (ButtonRead == (1<<13))			//tests to see if the button has been pressed
			{
				Button_Bounce_Counter += 1;
			} else 
			{
				Button_Bounce_Counter -= 1;
			}
		}
		
		if(Button_Bounce_Counter > 60)
		{
			return 1;
		} else if (Button_Bounce_Counter < 20)
		{
			return 0;
		} 
	}
}
