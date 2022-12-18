/*
 * WifiESP8266ATCom.cpp
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CommunicationTest.h>
WifiESP8266ATCom::WifiESP8266ATCom(UART_HandleTypeDef *uart,GPIO_TypeDef * resetGpio,uint16_t resetPin) {
	this->uart=uart;
	this->state=new ESP8266CommunicationTest(this);
	this->resetPin=resetPin;
	this->resetGpio=resetGpio;
	//this->state->setParent(this);
	this->nextState=nullptr;
}

WifiESP8266ATCom::~WifiESP8266ATCom() {
}
int WifiESP8266ATCom::Initialized(){
	this->Reset();
	return 0;
}
void WifiESP8266ATCom::Main(){
	if(this->nextState!=nullptr){
		delete this->state;
		this->state=this->nextState;
		//this->state->setParent(this);
		this->nextState=nullptr;
	}
	if(this->state!=nullptr && this->nextState==nullptr){
		this->state->main();
	}

}
int WifiESP8266ATCom::SendData(uint8_t *data,uint32_t size){
	HAL_UART_Transmit_IT(this->uart, data, size);
	return 0;
}
int WifiESP8266ATCom::addUartData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->uartData.push_back(*data);
	}
	return 0;
}
uint8_t*  WifiESP8266ATCom::getUartData(int *size){
	*size=this->uartData.size();
	if(*size>0)
		return this->uartData.data();
	else{
		return nullptr;
	}


}
void WifiESP8266ATCom::clearUartData(){
	this->uartData.clear();
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
	this->time++;
	if(this->nextState==nullptr){
		this->state->timerInterrupt();
	}
}
void WifiESP8266ATCom::wait(uint32_t ms){
	this->time=0;
	while(this->time<ms){
	}
}
void WifiESP8266ATCom::ChangeState(ESP8266State *state){
	this->nextState=state;
}
