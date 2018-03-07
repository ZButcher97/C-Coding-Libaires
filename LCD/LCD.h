#ifndef _LCD_H_
#define _LCD_H_
#define LCD_PORT	GPIOD
#define LCD_RS_pin	11
#define LCD_RW_pin	12
#define LCD_E_pin		13

#define LCD_D0_pin	0



#define LCD_LINE1		0x80
#define LCD_LINE2		0xc0

#define LCD_set_RS()	LCD_PORT->BSRR=(1u<<LCD_RS_pin)
#define LCD_clr_RS()	LCD_PORT->BSRR=(1u<<(LCD_RS_pin+16))
#define LCD_set_RW()	LCD_PORT->BSRR=(1u<<LCD_RW_pin)
#define LCD_clr_RW()	LCD_PORT->BSRR=(1u<<(LCD_RW_pin+16))
#define LCD_set_E()		LCD_PORT->BSRR=(1u<<LCD_E_pin)
#define LCD_clr_E()		LCD_PORT->BSRR=(1u<<(LCD_E_pin+16))

#define LCD_CLR()		cmdLCD(0x01)

#define LCD_set_bus_input()		LCD_PORT->MODER&=~(0xffff<<(2*LCD_D0_pin))
#define LCD_set_bus_output()	LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin))

#include <stm32f4xx.h>

void LCD_initalise(void);																		//This is used to initalise the LCD ports and registers so the above functions can be used in a program.
void LCD_clearLCD(void);																		//Function word to clear the LCD
void LCD_delayus(unsigned int us);													//blocking delay for LCD, argument is approximate number of micro-seconds to delay. - Input is time in seconds
void LCD_WaitLcdBusy(void); 																//Waits until the lcd is not busy. - input none
void LCD_set_data(unsigned char d);													//Sets the data onto the data lines. - input is the data
void LCD_strobe(void); 																			//10us high pulse on LCD enable line. - Input none
void LCD_cmdLCD(unsigned char cmd);													//sends a byte to the LCD control register. - Input comand word 
void LCD_putChr(unsigned char put);													//sends a char to the LCD display
void LCD_putString(char put[16]);														//sends a string to the LCD display
void LCD_putInt(float Number); 															//This function is able to convert a floting point from 9.999 down to 0.001 to a string
void LCD_putFromRight(int Line, char put[16]);							//This funciton will put a string from the right side of the LCD on the line specified.

void LCD_AnalougeLine(float MaxValue, float CurrentValue);	
																												/*		This funciton will display a analouge repersentation of a number passed through. Currently this is set to display the 
																															Line from the right on line 2. I has 12 individual symbols and will repersent any set of data when the max and current 
																															Value are passed through. 
																												*/
int LCD_ThouFind(int Number);																//This funciton returns the digit in the thousants position of the number sent
int LCD_HundFind(int Number);																//This funciton returns the digit in the hundreds position of the number sent
int LCD_TensFind(int Number);																//This funciton returns the digit in the tens position of the number sent

#endif
