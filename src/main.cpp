#include "mbed.h"
#include "stm32f4discovery_hal.h"
#include "stm32f4discovery_gpio.h"
#include "dwt_stm32_delay.h"
#include "dht.h"

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t sum, RH, TEMP;
int temp_low, temp_high, rh_low, rh_high;
char temp_char1[2], temp_char2, rh_char1[2], rh_char2;

GPIO_InitTypeDef GPIO_InitStruct;
void set_gpio_output (void)
{
	/*Configure GPIO pin output: PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
void set_gpio_input (void)
{
	/*Configure GPIO pin input: PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
//.......................................INICIALIZATION..........................................
 
void DHT22_start (void)
{
	set_gpio_output ();  // set the pin as output
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, 0);   // pull the pin low
	DWT_Delay_us (500);   // wait for 500us
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, 1);   // pull the pin high
	DWT_Delay_us (30);   // wait for 30us
	set_gpio_input ();   // set as input
}
 //....................................RESPONSE....................................................
void check_response (void)
{
	DWT_Delay_us (40);  
	if (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1))) {
	
		DWT_Delay_us (80);
		if ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1))) //check = 1;
  }
	while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)));   // wait for the pin to go low
}
 //......................................READ.......................................................
uint8_t read_data (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)));   // wait for the pin to go high
		DWT_Delay_us (40);   // wait for 40 us
		if ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)) == 0)   // if the pin is low 
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)));  // wait for the pin to go low
	}
	return i;
}
 
int main(void)
{
 
 
  while (1)
  {
  
		DHT22_start ();
		check_response ();
		Rh_byte1 = read_data ();
		Rh_byte2 = read_data ();
		Temp_byte1 = read_data ();
		Temp_byte2 = read_data ();
		sum = read_data();
		if (sum == (Rh_byte1+Rh_byte2+Temp_byte1+Temp_byte2))
		{
			TEMP = ((Temp_byte1<<8)|Temp_byte2);
			RH = ((Rh_byte1<<8)|Rh_byte2);
		}
    temp_low = TEMP/10;
		temp_high = TEMP%10;
		
		rh_low = RH/10;
		rh_high = RH%10;

 
 
 	
 
		HAL_Delay (1000);
  }
  
 
}