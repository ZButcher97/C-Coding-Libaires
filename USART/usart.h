#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>

#define USART_MODULE	USART3
#define USART_PORT		GPIOD
#define USART_TX_pin	8
#define USART_RX_pin	9
#define BAUDRATE			9600
#define WordLength8Bit 12
#define ParityOn 10
#define ParityType 9



void USART_Intialise(void); 																		//This function is used to initalise the registers and ports for the USART pins.
void USART_SendChar(unsigned char d);														// This function is used to send a character via USART pin Tx
void USART_SendString (const char *text);												// This function is used to send a string of characters via USART pin Tx
void USART_Parity_Setup(int ParityOnOff, int ParityOddEven);		// This function is used to set up the parity for the data
void USART_BaudRate(int BaudRate);															// This function will change the baud rate of the data

int USART_getChar(void);
int USART_getData(void);
#endif
