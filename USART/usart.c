#include "usart.h"
/* 
		This program has been written to set up the USART pins and functions.
		
		When included in a program, remember to include the initalise function to set up the registers/ports.
		This program was designed specificlly for the STM32-F429Zi
		
		
		
		Inturrputs for USART in RM0090 pg. 1009
		Parity for USART in RM0090 pg. 994
*/



//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void USART_Intialise(void)//This function is used to initalise the registers and ports for the USART pins.
{

	unsigned char i1,i2;
		SystemCoreClockUpdate();
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;					//usart port clock enable
	
	USART_PORT->MODER&=~(													//clear pin function bits
		(3u<<(2*USART_TX_pin))
		|(3u<<(2*USART_RX_pin))
			);
	USART_PORT->MODER|=(													//reset pin function bits (alternate function)
		(2u<<(2*USART_TX_pin))
		|(2u<<(2*USART_RX_pin))
			);
	
	i1=USART_TX_pin/8;
	i2=USART_RX_pin/8;

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(USART_TX_pin-(i1*8))));
	USART_PORT->AFR[i1]|=(0x07<<(4*(USART_TX_pin-(i1*8))));
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(USART_RX_pin-(i2*8))));
	USART_PORT->AFR[i2]|=(0x07<<(4*(USART_RX_pin-(i2*8))));
	
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;						//usart clock enable
	USART_MODULE->CR1|=(													//USART CONFIG
			USART_CR1_TE															//transmit enable
			|USART_CR1_RE															//receive enable
			|USART_CR1_UE															//usart main enable bit
				);
	USART_MODULE->BRR=SystemCoreClock/BAUDRATE;		//set baud rate
	
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void USART_SendChar(unsigned char d)// This function is used to send a character via USART pin Tx
{
	while(!(USART_MODULE->SR&USART_SR_TC));				/*wait for transmission complete. SR is the status register, SR_TC is the transmission complete bit. 
																									Once the previous word is sent, the next data can be written to the data register. 
																								*/
	USART_MODULE->DR=d;														//write byte to usart data register
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void USART_SendString (const char *text)// This function is used to send a string of characters via USART pin Tx
{ 
	unsigned  int idx = 0; 
	while (text[idx]) { 
		USART_SendChar(text[idx++]); 
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


void USART_Parity_Setup(int ParityOnOff, int ParityOddEven) // This function is used to set up the parity for the data
{
	if(ParityOnOff == 1)
	{
		USART_MODULE->CR1 = (0x01<<ParityOn); 			//Turns Parity check on
	} else {
		USART_MODULE->CR1 = (0x00<<ParityOn);				//Turns Parity check off
	}
	if(ParityOddEven == 1)
	{
		USART_MODULE->CR1 = (0x01<<ParityType); 		// Sets Parity to Odd
	} else {
		USART_MODULE->CR1 = (0x00<<ParityType); 		// Sets Parity to Even
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void USART_BaudRate(int BaudRate) // This function will change the baud rate of the data
{
	USART_MODULE->BRR=SystemCoreClock/BaudRate;		//set baud rate
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
int USART_getChar(void) //This function will read a transmitted character through USART and echo the character back on the transmit line
{
	signed int get_Char;
	do
	{
		get_Char = USART_getData(); 								//Checks the status of the recieving data. If all data is there, function will reture data, if not, -1 is returned.
	} 	while(get_Char == -1);
	USART_SendChar(get_Char); 										//Echo the data back
	return get_Char;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
int USART_getData(void) //This function will read the data on the USART data register
{
	if(USART2->SR & USART_SR_RXNE) 								// if the satus register and satue register(data ready to read) are high. 
	{
		return USART2->DR;
	} else {
		return -1;
	}
		
}
