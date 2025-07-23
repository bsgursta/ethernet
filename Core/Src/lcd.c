//used to program BuyDisplay OLEDs

#include <lcd.h>
#include "main.h"
#include "stm32f7xx_hal.h"

HAL_StatusTypeDef lcd_transfer(uint8_t data){
	//pull CS low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

	//transmit the data

	HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi2, &data, 1, 1000);
	if (status == HAL_OK){

	}
	//pull CS high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

	return HAL_OK;

}

//Reset lcd settings
HAL_StatusTypeDef lcd_reset(void){
	//pull RES pin down
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

	//pull RES pin up
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	return HAL_OK;
}

//turn LCD on to match RAM data
HAL_StatusTypeDef  lcd_ON(void){

	//lcd on data
	uint8_t data = 0xAF;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);


	return lcd_transfer(data);

}

//force all pixels on screen to turn on
HAL_StatusTypeDef  lcd_ALLpixels(void){

	//lcd on data
	uint8_t data = 0xA5;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);


	return lcd_transfer(data);

}

//only display pixels saved in RAM when lcd ON
HAL_StatusTypeDef  lcd_RAMpixels(void){

	//lcd on data
	uint8_t data = 0xA4;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);


	return lcd_transfer(data);
}


//turn lcd OFF
HAL_StatusTypeDef  lcd_OFF(void){

	//lcd off data
	uint8_t data = 0xAE;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

	return lcd_transfer(data);
}




//set page(horizontal) addressing after R/W a byte of SPI data
HAL_StatusTypeDef setPageaddressing(){
	uint8_t data = 0x20;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);


	return lcd_transfer(data);
}

//set vertical addressing after R/W a byte of SPI data
HAL_StatusTypeDef setVerticaladdressing(){

	uint8_t data = 0x21;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);


	return lcd_transfer(data);
}


//set the page address for writing pixels
//page number must be between 0-15
HAL_StatusTypeDef  lcd_setpage_address(int page_num){

	//convert int to uint8
	uint8_t pg_num = page_num;
	//data byte for page address
	uint8_t data = 0xB0 | pg_num;

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

	return lcd_transfer(data);
}
/*
 * seg remap = 1 -> D7 to D0
 * seg rempa = 0 -> D0 to D7
 *
 * */
HAL_StatusTypeDef lcd_segRemap(int value){
	if (value == 1){

		uint8_t data = 0xA1;

		return lcd_transfer(data);
	}
	else if(value == 0){
		uint8_t data = 0xA0;

		return lcd_transfer(data);
	}
	else{
		return HAL_ERROR;
	}

}

//set the column address for writing pixels
// must be between 0 and 127, there are 128 columns
HAL_StatusTypeDef  lcd_setcolumn_address(int column_num){

	if(column_num >= 128 || column_num < 0){
		return HAL_ERROR;
	}

	uint8_t num_conv = (uint8_t) column_num;

	uint8_t high_address = 0b00010000;
	uint8_t low_address = 0x00;

	high_address = high_address | ((num_conv >> 4) & 0x0F);
	low_address = low_address | (num_conv & 0x0F);

	//pull A0 low (PD7)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

	if(lcd_transfer(high_address) != HAL_OK){
		return HAL_ERROR;
	}
	if(lcd_transfer(low_address) != HAL_OK){
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef lcd_writeRAM(uint8_t data){
	//pull A0 high
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

	return lcd_transfer(data);
}

//initialize SPI protocol with 4 wire setup
//includes SCL,SI,A0, and /CS
//Pins: PB10 -> SCK, PC3 -> SI(MOSI), RES -> PC0, A0(D/C) -> PD7, CS -> PA3
HAL_StatusTypeDef lcd_4SPI_init(){
	//pull CS high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

	//pull RES pin up
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	//dummy transmission for SPI
	lcd_ON();
	lcd_OFF();

	return HAL_OK;

}

