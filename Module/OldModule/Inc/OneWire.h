/*
 * OneWire.h
 *
 *  Created on: 9 sie 2022
 *      Author: Daniel
 */

#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

uint8_t  OneWireReset(UART_HandleTypeDef *huart);
void OneWireSetBaudrate(UART_HandleTypeDef *huart,uint32_t baudrate);
void OneWireWriteBit(UART_HandleTypeDef *huart,uint8_t value);
uint8_t OneWireReadBit(UART_HandleTypeDef *huart);
#endif /* INC_ONEWIRE_H_ */
