#include "main.h"
#include "Time_Delays.h"
#include "Clk_Config.h"

#include "LCD_Display.h"

#include <stdio.h>
#include <string.h>

#include "Small_7.h"
#include "Arial_9.h"
#include "Arial_12.h"
#include "Arial_24.h"

#include "stm32f4xx_ll_crc.h"


//Accelerometer Sensor I2C Address
#define ACCELADR 0x98

//EEPROM I2C Address
#define EEPROMADR 0xA0
//GPIO
void configure_gpio(void);

//Joystick Configuration
void joystick_configure(void);
uint32_t joystick_left(void);
uint32_t joystick_right(void);
uint32_t joystick_centre(void);
uint32_t joystick_up(void);
uint32_t joystick_down(void);

//I2C
void i2c_1_configure(void);

//Accelerometer
void config_accelerometer(void);
char read_accelerometer(void);

//Display functions
void display_tilt(uint8_t sample); //convenience function to display tilt register
void packet_display(char* array, char i); //display contents of packet after page-mode read
void pckt_down(uint8_t* dest, uint8_t* src, uint8_t* length, int count, uint8_t sample);
//EEPROM functions
void eeprom_write(uint8_t data);
uint8_t eeprom_read(void);
void eeprom_write_page_mode(uint8_t data);
void eeprom_read_page_mode(char* array);



int main(void){
//Init
  SystemClock_Config();/* Configure the system clock to 84.0 MHz */
SysTick_Config_MCE2(us);

// configure the LCD
Configure_LCD_Pins () ;
Configure_SPI1 () ;
Activate_SPI1 () ;
Clear_Screen () ;
Initialise_LCD_Controller () ;
set_font (( unsigned char *) Arial_9 );
//Configure GPIO
configure_gpio();
joystick_configure();
i2c_1_configure(); //Configure I2C and set up the GPIO pins it uses
config_accelerometer();
uint8_t sample = 0; //int to store tilt register value
	
char packet[50] = {0}; //50-byte array to contain copies of the tilt register value
uint8_t dest[4] = {0xcc, 0xcc, 0xcc, 0xcc};
uint8_t src[4] = {0xdd, 0xdd, 0xdd, 0xdd};
uint8_t length[2] = {0x00, 0x32};
int count = 2;

while(1){
if (joystick_centre() == 1){

sample = read_accelerometer(); //Reads accelerometer sensor
put_string(0,0,"             "); //Report successful data read
put_string(0,0,"Sampled");
put_string(0,15,"             ");
LL_mDelay(500000);
display_tilt(sample);
}
else if (joystick_right() == 1) {
eeprom_write_page_mode((uint8_t)sample); //single-byte write to EEPROM
put_string(0,0,"             "); //Report successful data write
put_string (0 ,0 , "Written");
put_string(0,15,"             ");
LL_mDelay(500000);
display_tilt(sample);
}
else if (joystick_left() == 1) {
eeprom_read_page_mode(packet);
put_string(0,0,"             "); //Report successful data read
put_string (0 ,0 , "Retrived");
put_string(0,15,"             ");
LL_mDelay(500000);
packet_display(packet, 0);
put_string(0,0,"             ");
put_string(0,0,"Tilt Reg:");
}

else if(joystick_up() == 1){  //SCROLL UP
//display first copy of tilt register:

	put_string(0,0,"             ");
	put_string(0,15,"             ");
	LL_mDelay(500000);

	count = count-1;
	pckt_down( dest, src, length, count, sample);
}

else if(joystick_down() == 1){  //SCROLL DOWN
//display first copy of tilt register:
	put_string(0,0,"             ");
	put_string(0,15,"             ");
	LL_mDelay(500000);
	
	count = count+1;
	pckt_down( dest, src, length, count, sample);
}
}
}


uint32_t joystick_left(void) {
//Returns 1 if the joystick is pressed left, 0 otherwise
return (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_1));
}

uint32_t joystick_right(void) {
//Returns 1 if the joystick is pressed right, 0 otherwise
return (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_0));
}
uint32_t joystick_centre(void) {
//Returns 1 if the joystick is pressed in the centre, 0 otherwise
return (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_5));
}
uint32_t joystick_up(void) {
//Returns 1 if the joystick is pressed up, 0 otherwise
return (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_4));
}
uint32_t joystick_down(void) {
//Returns 1 if the joystick is pressed down, 0 otherwise
return (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0));
}

/* A function to display the tilt register value */
void display_tilt(uint8_t sample){
char data_bin[8] = {0};
char outputString[18]; //Buffer to store text in for LCD
//Converts 8 bits into array for display purposes
for(int j = 0; j < 8; j++){
       data_bin[j] = (sample & (0x80 >> j)) > 0;
}
put_string(0,0,"             ");
put_string(0,0,"Tilt Reg");
put_string(0,15,"             ");
sprintf(outputString,"%x%x%x%x%x%x%x%x", data_bin[0], data_bin[1], data_bin[2], data_bin[3], data_bin[4], data_bin[5], data_bin[6], data_bin[7]);
put_string(0,15,outputString);
}

void configure_gpio(void){
//Configures the GPIO pins by enabling the peripherial clocks on the ports uses by the shield
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
}

void i2c_1_configure(void){
//Enable I2C1 Clock
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
 
// Configure SCL as: Alternate function, High Speed, Open Drain, Pull Up
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
        LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
        // Configure SDA as: Alternate, High Speed, Open Drain, Pull Up
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
        LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
 
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
 
        LL_I2C_Disable(I2C1);
        LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);
        LL_I2C_ConfigSpeed(I2C1, 84000000, 100000, LL_I2C_DUTYCYCLE_2);  //set speed to 100 kHz
        LL_I2C_Enable(I2C1);
}

void config_accelerometer(void){
        //Sets accelerometer to active mode
        LL_I2C_GenerateStartCondition(I2C1); //START
        while(!LL_I2C_IsActiveFlag_SB(I2C1));

        LL_I2C_TransmitData8(I2C1, ACCELADR); //ADDRESS + WRITE
        while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
        LL_I2C_ClearFlag_ADDR(I2C1);

        LL_I2C_TransmitData8(I2C1, 0x07); //Transmit mode register address
        while(!LL_I2C_IsActiveFlag_TXE(I2C1));

        LL_I2C_TransmitData8(I2C1, 0x01); //Set device to active mode (writes a 1 to mode register bit 0)
        while(!LL_I2C_IsActiveFlag_TXE(I2C1));

        LL_I2C_GenerateStopCondition(I2C1);       //STOP
}

void joystick_configure(void){
//This function configures all the GPIO pins that are connected to the joystick on the mbed shield
LL_GPIO_SetPinMode (GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT); //set PA4 as Input
LL_GPIO_SetPinPull (GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);     //set PA4 as NO pull
LL_GPIO_SetPinMode (GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT); //set PB0 as Input
LL_GPIO_SetPinPull (GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);     //set PB0 as NO pull
LL_GPIO_SetPinMode (GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT); //set PB5 as Input
LL_GPIO_SetPinPull (GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);     //set PB5 as NO pull
LL_GPIO_SetPinMode (GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT); //set PC0 as Input
LL_GPIO_SetPinPull (GPIOC, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);     //set PC0 as NO pull
LL_GPIO_SetPinMode (GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT); //set PC1 as Input
LL_GPIO_SetPinPull (GPIOC, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);     //set PC1 as NO pull
}

char read_accelerometer(void){

  //uint16_t data = 0;
  uint8_t data = 0;

  LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, ACCELADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);

  LL_I2C_TransmitData8(I2C1, 0x03); //Transmit tilt register address
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));

  LL_I2C_GenerateStartCondition(I2C1); //RE-START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, ACCELADR+1); //ADDRESS + READ
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);

  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK); //NACK INCOMING DATA
  while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
  data = LL_I2C_ReceiveData8(I2C1);  //Store the data received and clears the RXNE flag

  LL_I2C_GenerateStopCondition(I2C1);    //STOP

  return data;
}


void eeprom_write(uint8_t data){
LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit high byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit low byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, data); //Data
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_GenerateStopCondition(I2C1);    //STOP
}

uint8_t eeprom_read(void){
  uint8_t data;
LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit high byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit low byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_GenerateStartCondition(I2C1); //RE-START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR+1); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK); //NACK INCOMING DATA
  while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
  data = LL_I2C_ReceiveData8(I2C1);  //Store the data received and clears the RXNE flag
LL_I2C_GenerateStopCondition(I2C1);    //STOP

  return data;
}
void eeprom_write_page_mode(uint8_t data){
     //write 50 copies of data in page mode (i.e. 32, then the remainder)
 
LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit high byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit low byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
for (int i = 0; i < 31; i++){
LL_I2C_TransmitData8(I2C1, data); //Data
while(!LL_I2C_IsActiveFlag_TXE(I2C1));
}
LL_I2C_GenerateStopCondition(I2C1);    //STOP
LL_mDelay(5000); //Delay
//Page 2
LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_TransmitData8(I2C1, 0x20); //Transmit high byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, 0x20); //Transmit low byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
for (int i = 0; i < 18; i++){
LL_I2C_TransmitData8(I2C1, data); //Data
while(!LL_I2C_IsActiveFlag_TXE(I2C1));
}
LL_I2C_GenerateStopCondition(I2C1);    //STOP
}

void eeprom_read_page_mode(char* array){
LL_I2C_GenerateStartCondition(I2C1); //START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit high byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_TransmitData8(I2C1, 0x00); //Transmit low byte
  while(!LL_I2C_IsActiveFlag_TXE(I2C1));
LL_I2C_GenerateStartCondition(I2C1); //RE-START
  while(!LL_I2C_IsActiveFlag_SB(I2C1));
LL_I2C_TransmitData8(I2C1, EEPROMADR+1); //ADDRESS + WRITE
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_ClearFlag_ADDR(I2C1);
LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK); //ACK INCOMING DATA
  while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
array[0]= LL_I2C_ReceiveData8(I2C1);
LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK); //ACK INCOMING DATA
  while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
array[1] = LL_I2C_ReceiveData8(I2C1);

LL_I2C_GenerateStopCondition(I2C1);    //STOP
}

/* A function to display either the first or second copy of the tilt register value
   depending on joystick up/down presses */
void packet_display(char* array, char i) {
        //as per display_tilt:
        char data_bin[8] = {0};
char outputString[18]; //Buffer to store text in for LCD
      //Converts 8 bits into array for display purposes
for(int j = 0; j < 8; j++){
       data_bin[j] = (array[i] & (0x80 >> j)) > 0;
}
put_string(0,0,"             ");
put_string(0,15,"             ");
sprintf(outputString,"%x%x%x%x%x%x%x%x", data_bin[0], data_bin[1], data_bin[2], data_bin[3], data_bin[4], data_bin[5], data_bin[6], data_bin[7]);
put_string(0,15,outputString);

} //end packet_display

void pckt_down(uint8_t* dest, uint8_t* src, uint8_t* length, int count, uint8_t sample){

	char outputString[18]; //Buffer to store text in for LCD
	
				if(count != 3){
					count = count + 1;
				}
			
				if(count < 0){
						count = 0;
				}
				
				if(count==0){

				put_string(0,0,"dest");
				sprintf(outputString, "%x%x%x%x", dest[0], dest[1], dest[2], dest[3]);
				put_string(0,15, outputString);
				}
				
				if(count==1){

				put_string(0,0,"src");
				sprintf(outputString, "%x%x%x%x", src[0], src[1], src[2], src[3]);
				put_string(0,15, outputString);
				}
				
				if(count==2){
				LL_mDelay(500000);	
				put_string(0,0,"length");
				sprintf(outputString, "%x%x", length[0], length[1]);
				put_string(0,15, outputString);
				}
				
				if(count==3){
					LL_mDelay(500000);
					display_tilt(sample);
					
				}
				
				if(count > 3){
					count = 3;
				}

}
