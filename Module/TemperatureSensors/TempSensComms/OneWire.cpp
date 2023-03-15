/*
 * OneWire.cpp
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */
#include "stdio.h"
#include <OneWire.h>

OneWire::OneWire() {
	// TODO Auto-generated constructor stub
}

OneWire::~OneWire() {
	// TODO Auto-generated destructor stub
}
void OneWire::SetBaudrate(uint32_t baudrate){
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
void OneWire::WriteBit(uint8_t *value){
	if (*value) {
		uint8_t data_out = 0xff;
		HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	}else{
		uint8_t data_out = 0x0;
	    HAL_UART_Transmit(huart, &data_out, 1, HAL_MAX_DELAY);
	}
}
uint32_t OneWire::Reset(){
	uint8_t data_out = 0xF0;
	uint8_t data_in = 0;
	uint8_t result=0;
	this->SetBaudrate(9600);
	HAL_UART_Transmit(this->huart, &data_out, 1, HAL_MAX_DELAY);
	HAL_UART_Receive(this->huart, &data_in, 1, HAL_MAX_DELAY);
	this->SetBaudrate(115200);
	if (data_in != 0xF0)
		result= HAL_OK;
	else
		result=HAL_ERROR;
	return result;
}
void OneWire::Write(uint8_t *data,uint32_t bitSize){
	uint32_t byte,bit;
	uint8_t dataToSend;
	for(uint32_t i=0;i<bitSize;i++){
		byte=i/8;
		bit=i%8;
		dataToSend=(data[byte]>>bit)&0x01;
		this->WriteBit(&dataToSend);
	}
}
uint8_t OneWire::Read(){
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
void OneWire::InterfaceSet(UART_HandleTypeDef *huart){
	this->huart=huart;
	this->SetBaudrate(115200);
}
