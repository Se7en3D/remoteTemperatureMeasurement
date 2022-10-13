/*
 * WifiESP8266ATCom.cpp
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/WifiESP8266ATCom.h>

WifiESP8266ATCom::WifiESP8266ATCom(UART_HandleTypeDef *uart,GPIO_TypeDef * resetGpio,uint16_t resetPin) {
	this->uart=uart;
	this->state=new ESP8266CommunicationTest();
	this->resetPin=resetPin;
	this->resetGpio=resetGpio;
	this->state->setParent(this);
}

WifiESP8266ATCom::~WifiESP8266ATCom() {
	// TODO Auto-generated destructor stub
}
int WifiESP8266ATCom::Initialized(){
	this->Reset();
	return 0;
}
void WifiESP8266ATCom::Main(){
	this->state->main();

}
int WifiESP8266ATCom::SendData(uint8_t *data,uint32_t size){
	HAL_UART_Transmit_IT(uart, data, size);
	return 0;
}
int WifiESP8266ATCom::addData(uint8_t *data,uint32_t size){
	return this->state->addData(data, size);
}
int WifiESP8266ATCom::GetFrameFromBuffer(uint8_t *data){
	return 0;
}
void WifiESP8266ATCom::Reset(){
	HAL_GPIO_WritePin(resetGpio, resetPin, GPIO_PIN_RESET);
	wait(100);
	HAL_GPIO_WritePin(resetGpio, resetPin, GPIO_PIN_SET);
}
void WifiESP8266ATCom::TimeInterrupt(){
	time++;
	this->state->timerInterrupt();
}
void WifiESP8266ATCom::wait(uint32_t ms){
	this->time=0;
	while(this->time<ms){
	}
}
void WifiESP8266ATCom::ChangeState(ESP8266State *state){
	this->state=state;
}
