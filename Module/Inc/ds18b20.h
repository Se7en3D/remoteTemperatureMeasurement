/*
 * ds18b20.h
 *
 *  Created on: 12 sie 2022
 *      Author: Daniel
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_
#include "OneWire.h"
#define DS18B20_SEARCH_ROM 0xF0
typedef struct ds18b20 ds18b20;
typedef enum ds18b20state ds18b20state;
struct ds18b20{
	UART_HandleTypeDef *huart;
	void(*initialization)(UART_HandleTypeDef *);
	uint8_t romNO[8];
	uint32_t romAddedBit;
};
ds18b20 *ds18b20CreateClass(UART_HandleTypeDef *huart);
void ds18b20Init(ds18b20* this,
		void(*initialization)(ds18b20 *));
void ds18b20Initialization(ds18b20 *this);
static void ds18b20SendData(UART_HandleTypeDef *huart,uint8_t command);
static void ds18b20AddBitToRom(ds18b20 *this,uint8_t bit);

	//FUNKCJE TYMCZASOWE
unsigned char docrc8(ds18b20* this, unsigned char value);
#endif /* INC_DS18B20_H_ */
