/*
 * OneWire.c
 *
 *  Created on: 9 sie 2022
 *      Author: Daniel
 */


#include "stm32f4xx_hal.h"
#include "OneWire.h"

uint8_t  OneWireReset(UART_HandleTypeDef *huart){
	uint8_t data_out = 0xF0;
	uint8_t data_in = 0;
	uint8_t result=0;
	OneWireSetBaudrate(huart,9600);
	HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	HAL_UART_Receive(huart, &data_in, 1, HAL_MAX_DELAY);
	OneWireSetBaudrate(huart,115200);
	if (data_in != 0xF0)
		result= HAL_OK;
	else
		result=HAL_ERROR;
//	printf("%d",(int)result);
	return result;
}
void OneWireWriteBit(UART_HandleTypeDef *huart,uint8_t value){
	  if (value) {
	      uint8_t data_out = 0xff;
	    HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	  } else {
	      uint8_t data_out = 0x0;
	    HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	  }
}
uint8_t OneWireReadBit(UART_HandleTypeDef *huart){
	  uint8_t data_out = 0xFF;
	  uint8_t data_in = 0;
	  while(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) == SET) {
		  uint8_t tempBuffer;
		  HAL_UART_Receive(huart, &tempBuffer, 1, HAL_MAX_DELAY);
	  }
	  HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	  HAL_UART_Receive(huart, &data_in, 1, HAL_MAX_DELAY);
	  //printf("Read data %x\n",data_in);
	  return data_in & 0x01;
}
void OneWireSetBaudrate(UART_HandleTypeDef *huart,uint32_t baudrate){
	huart->Instance = UART5;
	huart->Init.BaudRate = baudrate;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_HalfDuplex_Init(huart) != HAL_OK)
	{
		while(1){

		}
	}
}
