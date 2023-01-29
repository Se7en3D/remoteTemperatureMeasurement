/*
 * WifiESP8266ATCom.cpp
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CommunicationTest.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CipSend.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
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
	if(this->uartReceivedBuffer.size()>MAX_WIFI_RECEIVED_BUFFER){
		this->uartReceivedBuffer.erase(MAX_WIFI_RECEIVED_BUFFER/2,0);
	}

}
int WifiESP8266ATCom::SendData(uint8_t *data,uint32_t size){
	if(this->state->readyToSend() && nextState==nullptr){
		ESP8266CipSend *nextState=new ESP8266CipSend(this);
		nextState->sendData(reinterpret_cast<const char*>(data), size);
		this->nextState=nextState;
		return 0;
	}
	//HAL_UART_Transmit_IT(this->uart, data, size);
	return 1;
}
int WifiESP8266ATCom::SendCommand(uint8_t *data,uint32_t size){
	HAL_UART_Transmit_IT(this->uart, data, size);
	return 0;
}
int WifiESP8266ATCom::addUartData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		//this->uartData.push_back(*data);
		this->uartReceivedBuffer.append(reinterpret_cast<char*>(data));
	}
	return 0;
}
const char *  WifiESP8266ATCom::getUartData(int *size){
	*size=this->uartReceivedBuffer.size();
	if(*size>0){
		//uint8_t* data=this->uartData.data();
		//return data;
		return this->uartReceivedBuffer.c_str();
	}else{
		return nullptr;
	}


}
void WifiESP8266ATCom::clearUartData(){
	//for(unsigned char &data:this->uartData ){
	//	data=0;
	//}
	this->uartReceivedBuffer.clear();
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
