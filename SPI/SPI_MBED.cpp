#include "mbed.h"

SPI spi(PA_7, PA_6, PA_5);      // Ordered as: mosi, miso, sclk could use forth parameter ssel
DigitalOut cs(PC_6);            // Chip Select for Basic Outputs to illuminate Onboard FPGA DEO nano LEDs CN7 pin 1

int32_t SendReadSPI(int d);    //Read 4 Sliding switches on FPGA (Simulating OPTO-Switches from Motor(s)
void PrintVal(int sw_val);

//NBB the following line for F429ZI !!!!
DigitalIn DO_NOT_USE(PB_12);    // MAKE PB_12 (D19) an INPUT do NOT make an OUTPUT under any circumstances !!!!! ************* !!!!!!!!!!!
                                // This Pin is connected to the 5VDC from the FPGA card and an INPUT is 5V Tolerant
BusIn BUTTONs (USER_BUTTON, D4, D3);
BusOut MCU_LEDs (D7, D6, D5);

//Ticker ticktock;
  
int main() {
    cs = 1;                     // Chip must be deselected, Chip Select is active LOW
    spi.format(8,0);            // Setup the DATA frame SPI for 16 bit wide word, Clock Polarity 0 and Clock Phase 0 (0)
    spi.frequency(1000000);     // 1MHz clock rate

	
		printf("TEST\n\r");
	//This sets up an 8 bit value to send over SPI
  int LED_Val = 0b00000000;
	//Value read from MISO SPI
	int Data_Read = 0;
	//Main program loop
	while(true)                 //Loop forever Knight Rider Display on FPGA
    {		
			//Sets the MSB 3 bits to the Button inputs
			LED_Val = BUTTONs << 5;
			//sends data and reads MISO
			Data_Read = SendReadSPI(LED_Val);
			//sets the MCU leds to show what buttons are being pressed
			MCU_LEDs = BUTTONs;
			//prints the MISO data read and outputs to PuTTY
			PrintVal(Data_Read);
			wait_ms(20);
    }
}

//Function to read back the state of the switches
int SendReadSPI(int d){
    int sw_val = 0;
    cs = 0;				//Select the device by seting chip select LOW
    spi.write(0);	//Command
    sw_val = spi.write(d); // & 0x0F;  Mask to read back lower 4bit nibble
    cs = 1 ;			//De-select the device by seting chip select HIGH
    return sw_val;    
}

void PrintVal(int sw_val)
{
	    if (sw_val&(1<<0)){ printf("Switch 0 :"); }
    if (sw_val&(1<<1)){ printf("Switch 1 :"); }
    if (sw_val&(1<<2)){ printf("Switch 2 :"); }
    if (sw_val&(1<<3)){ printf("Switch 3 :"); }
    if (sw_val>0)     { printf("\r\n");       }
}
