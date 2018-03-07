#include "lcd.h.h"
/* 
		This program has been written to set up communication with the included LCD screen.
		
		When included in a program, remember to include the initalise function to set up the registers/ports.
		This program was designed specificlly for the STM32-F429Zi
*/

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_initalise(void) //This is used to initalise the LCD ports and registers so the above functions can be used in a program.
{
		SystemCoreClockUpdate();
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;	//enable LCD port clock
	
	
			//CONFIG LCD GPIO PINS
		LCD_PORT->MODER&=~(					//clear pin direction settings
			(3u<<(2*LCD_RS_pin))
			|(3u<<(2*LCD_RW_pin))
			|(3u<<(2*LCD_E_pin))
			|(0xffff<<(2*LCD_D0_pin))
			);
	
	
	LCD_PORT->MODER|=(				//reset pin direction settings to digital outputs
			(1u<<(2*LCD_RS_pin))
			|(1u<<(2*LCD_RW_pin))
			|(1u<<(2*LCD_E_pin))
			|(0x5555<<(2*LCD_D0_pin))
		);

	LCD_PORT->OTYPER |=(			
			(0u<<(LCD_RS_pin))
			|(0u<<(LCD_RW_pin))
			|(0u<<(LCD_E_pin))
			|(0xFF<<(LCD_D0_pin))
		);

	
			//LCD INIT COMMANDS
	LCD_clr_RS();					//all lines default low
	LCD_clr_RW();
	LCD_clr_E();
	
	LCD_delayus(25000);		//25ms startup delay
	LCD_cmdLCD(0x38);	//Function set: 2 Line, 8-bit, 5x7 dots
	//cmdLCD(0x28);	//Function set: 2 Line, 4-bit, 5x7 dots
	LCD_cmdLCD(0x0d);	//Display on, Cursor blinking command
	LCD_cmdLCD(0x01);	//Clear LCD
	LCD_cmdLCD(0x06);	//Entry mode, auto increment with no shift
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_clearLCD(void) //Function word to clear the LCD
{
	LCD_cmdLCD(0x01); 
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_delayus(unsigned int us)	{	//blocking delay for LCD, argument is approximate number of micro-seconds to delay. - Input is time in seconds

	unsigned char i;
	while(us--)
	{
		for(i=0; i<SystemCoreClock/4000000; i++);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_WaitLcdBusy(void) //Waits until the lcd is not busy. - input none
{
	//Rs = 0, RW = 1, DB7 is the busy flag.
	
	int Data_Input = 1u;
	LCD_clr_RS();					//control command
	LCD_set_RW();					//read command
	
	LCD_PORT ->MODER&=~(0xffff<<(2*LCD_D0_pin));		//set pin direction settings to digital inputs
	
	while(Data_Input != 0u)
	{
		LCD_set_E();
		LCD_delayus(10);		//Raise E to read the data
		
		Data_Input = LCD_PORT -> IDR & (1u<<7);
		
		LCD_clr_E();				//Reset E to refresh DB7
		LCD_delayus(10);
	}
	
	LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin));			//set pin direction settings to digital outputs
	
	
	//lcd_delayus(3000);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_set_data(unsigned char d) //Sets the data onto the data lines. - input is the data
{
	LCD_PORT->BSRR=(0xff<<(LCD_D0_pin+16));	//clear data lines
	LCD_PORT->BSRR=(d<<LCD_D0_pin);					//put data on lines
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_strobe(void)		//10us high pulse on LCD enable line. - Input none
{
	LCD_delayus(10);
	LCD_set_E();
	LCD_delayus(10);
	LCD_clr_E();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_cmdLCD(unsigned char cmd)		//sends a byte to the LCD control register. - Input comand word 
{
	LCD_WaitLcdBusy();				//wait for LCD to be not busy
	LCD_clr_RS();					//control command
	LCD_clr_RW();					//write command
	LCD_set_data(cmd);		//set data on bus
	LCD_strobe();					//apply command
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_putChr(unsigned char put)	//sends a char to the LCD display
{
	LCD_WaitLcdBusy();				//wait for LCD to be not busy
	LCD_set_RS();					//text command
	LCD_clr_RW();					//write command
	LCD_set_data(put);		//set data on bus
	LCD_strobe();					//apply command
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_putString(char put[16])	//sends a string to the LCD display
{
	LCD_WaitLcdBusy();				//wait for LCD to be not busy
	LCD_set_RS();					//text command
	LCD_clr_RW();					//write command

	
	for(int i = 0; put[i]!='\0'; i++)
		{
		LCD_set_data(put[i]);		//set data on bus
		LCD_strobe();					//apply command
		}

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_putInt( float Number) //This function is able to convert a floting point from 9.999 down to 0.001 to a string
{
	char Thou;
	char Hund;
	char Tens;
	char Ones;
	
	input = input/1.235; //ask lecture why error occures without these two operations
	
	input = input*1.235;
	
	input *= 1000;
	
	if(input >= 1000)
	{
		
		for(int i = 1; (i-1)*1000 <= input; i++)
		{
			if(i*1000 > input )
			{
				i -=1;
				Thou = i + '0';
				input = input - (i*1000);
			}
		}
	} else {
		Thou = '0';
	}
	if(input >= 100)
	{
		
		for(int i = 1; (i-1)*100 <= input; i++)
		{
			if(i*100 > input )
			{
				i -=1;
				Hund = i + '0';
				input = input - (i*100);
			}
		}
	}else {
		Hund = '0';
	}
	if(input >= 10)
	{
		
		for(int i = 1; (i-1)*10 <= input; i++)
		{
			if(i*10 > input )
			{
				i -=1;
				Tens = i + '0';
				input = input - (i*10);
			}
		}
	}else {
		Tens = '0';
	}
	if(input >= 1)
	{
		
		for(int i = 1; (i-1) <= input; i++)
		{
			if(i > input )
			{
				i -= 1;
				Ones = i + '0';
				input = input - i;
			}
		}
	}else {
		Ones = '0';
	}
	
	char Output[5];
	Output[0] = Thou;
	Output[1] = '.';
	Output[2] = Hund;
	Output[3] = Tens;
	Output[4] = Ones;
	
	LCD_putString(Output);
	
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void LCD_putFromRight(int Line, char put[16])	//This funciton will put a string from the right side of the LCD on the line specified.
{
	LCD_WaitLcdBusy();				//wait for LCD to be not busy
	LCD_set_RS();					//text command
	LCD_clr_RW();					//write command
	
	int len = 0;
	int CurShift = 0;
	
	if(Line == 1)
		{
			LCD_cmdLCD(LCD_LINE1);
		} 
	
	else 
		{
			LCD_cmdLCD(LCD_LINE2);
			LCD_cmdLCD(0x14);
		}
	
	
	
	for(int i = 0; put[i]!='\0'; i++)
		{
			len += 1;
		}
		
		CurShift = 15-len;
		
		for(int i = 0; i != CurShift +1; i++)
		{
			LCD_cmdLCD(0x14);			
		}
		
			for(int i = 0; put[i]!='\0'; i++)
		{
			LCD_putChr(put[i]);
		}
		
		LCD_cmdLCD(LCD_LINE1);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*
		This funciton will display a analouge repersentation of a number passed through. Currently this is set to display the 
		Line from the right on line 2. I has 12 individual symbols and will repersent any set of data when the max and current 
		Value are passed through. 
*/

void LCD_AnalougeLine(float MaxValue, float CurrentValue) 
{
	if( CurrentValue == 1) 
		{
			CurrentValue = 35;
		}
	float LineDifference = MaxValue / 12;
	float NoLines = 0;
		
	char Output[5];

	for(float i = 0; CurrentValue >= LineDifference; ++i)
	{
		CurrentValue = CurrentValue - LineDifference;
		NoLines = i;
	}
	
	if (NoLines >= 6)
	{
		NoLines = NoLines - 6;
		for (int i = 0; i!= 6; i++)
		{
			if(i <= NoLines)
			{
				Output[5-i] = '=';
			} else {
				Output[5-i] = '-';
			}

		}
	} else {
			for (int i = 0; i!= 6; i++)
			{
				if(i <= NoLines)
				{
					Output[5-i] = '-';
				} else {
					Output[5-i] = ' ';
				}
			}
	} 
		LCD_putFromRight(2, Output); //Where is where the data is set to appear from the right on line 2.
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//




