#include "max7219.h"
#include "THIN_SS.h"

//message length
int messageLength;

//display buffer
unsigned char *dBuff;

void write_byte(uint8_t byte)
{
	    for(int i=0;i<8;i++)
          {
	    	 HAL_GPIO_WritePin(MAX7219_CLK_Port, MAX7219_CLK_Pin, 0);  // Pull CLK LOW
	    	 HAL_GPIO_WritePin(MAX7219_DIN_Port, MAX7219_DIN_Pin, byte&0x80);// Write one BIT data MSB first
             byte = byte<<1;  // shift the data to the left
             HAL_GPIO_WritePin(MAX7219_CLK_Port, MAX7219_CLK_Pin, 1);  // Pull CLK HIGH
           }
}


void write_max (uint8_t address, uint8_t data)
{
	HAL_GPIO_WritePin(MAX7219_CS_Port, MAX7219_CS_Pin, 0);   // Pull CS pin LOW
    write_byte(address);   //  write address
    write_byte(data);  //  write data
    HAL_GPIO_WritePin(MAX7219_CS_Port, MAX7219_CS_Pin, 1);  // Pull CS pin HIGH
}


void max7219_init(void)
{
 write_max(0x09, decoding);       //  no decoding
 write_max(0x0a, intensity);       //  brightness intensity
 write_max(0x0b, scanLimit);       //  scan limit = 8 LEDs
 write_max(0x0f, testDisplay);       //  no test display
 write_max(0x0c, 0x01);       //  power down = 0 ，normal mode = 1
}

void max7219_clear(void)
{
	for (int a=0; a<=8*totalLedMaxtrixElements; a++){
		write_max(0x00, 0x00);
	}
}

void initBuffer(int messageLength){
	dBuff = calloc(messageLength*8, sizeof(char));
}

void fillBuffer(char* message){
	int row = 0;
	for (int y=0; y<8; y++){
		for (int x=0; x<messageLength; x++){
			dBuff[x+row] = displayFonts[(int)message[x]][y];
		}
		row +=messageLength;
	}
}

void max7219_8x8led_showBuffer(){
	int row = 0;
	for (int y=0; y<8; y++){
		HAL_GPIO_WritePin(MAX7219_CS_Port, MAX7219_CS_Pin, 0);   // Pull CS pin LOW
		for (int x=0; x<totalLedMaxtrixElements; x++){
			write_byte(y+1);
			write_byte(dBuff[x+row]);
		}
		row += messageLength;
		HAL_GPIO_WritePin(MAX7219_CS_Port, MAX7219_CS_Pin, 1);  // Pull CS pin HIGH
	}
}


void max7219_8x8led_shift_message(int shiftbits){
	int row = 0;
	for (int y=0; y<8; y++){
		unsigned char tempBuffer = dBuff[row];
		for (int x=0; x<messageLength; x++){
			if (x == messageLength - 1){
				dBuff[x+row] = dBuff[x+row]<<shiftbits | tempBuffer>>(8-shiftbits);
			}
			else{
				dBuff[x+row] = dBuff[x+row]<<shiftbits | dBuff[x+row+1]>>(8-shiftbits);
			}
		}
		row +=messageLength;
	}
}

void max7219_8x8led_freeBuffer(){
	free(dBuff);
}

void max7219_8x8led_display_message(char* message){
	messageLength = strlen(message);

	//init buffer
	initBuffer(messageLength);

	//fill buffer
	fillBuffer(message);

	//Show buffer
	max7219_8x8led_showBuffer();
}


void max7219_8x8led_display_message_continuous(char* message, int shiftDelay){
	messageLength = strlen(message);

	//init buffer
	initBuffer(messageLength);

	//fill buffer
	fillBuffer(message);

	//Show buffer
	max7219_8x8led_showBuffer();

	while(1){
		//delay
		HAL_Delay(shiftDelay);
		//shift buffer
		max7219_8x8led_shift_message(1);
		//Show buffer
		max7219_8x8led_showBuffer();
	}
}


