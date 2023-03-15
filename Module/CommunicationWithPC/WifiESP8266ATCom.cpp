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

WifiESP8266ATCom::WifiESP8266ATCom(UART_HandleTypeDef *uart,GPIO_TypeDef * resetGpio,uint16_t resetPin){
	this->uart=uart;
	this->state=new ESP8266CommunicationTest();
	this->resetPin=resetPin;
	this->resetGpio=resetGpio;
	this->time=0;
	//uartReceivedBridgeBuffer=new CircularBuffer<50,uint8_t>;
}

WifiESP8266ATCom::~WifiESP8266ATCom() {
	//if(uartReceivedBridgeBuffer!=nullptr){
	//	delete uartReceivedBridgeBuffer;
	//}
	if(state!=nullptr){
		delete state;
	}
}
int WifiESP8266ATCom::Initialized(){
	this->Reset();
	return 0;
}
void WifiESP8266ATCom::Main(){
	if(this->state==nullptr){
		return;
	}
	uint8_t tempBuffer;
	while(bridgeReceivedBuffer.getData(&tempBuffer)){
		this->uartReceivedBuffer+=tempBuffer;
	}
	if(this->state->readyToSendInit(this->time)){
		uint32_t size=0;
		const uint8_t *dataToSend=this->state->getInitialData(&size);
		if(dataToSend!=nullptr){
			HAL_UART_Transmit_IT(this->uart, (uint8_t*)dataToSend, size);
		}
		this->time=0;
	}
	ESP8266State *nextState=this->state->getNextState(uartReceivedBuffer);
	if(nextState!=nullptr){
		if(this->state->getStateName()!=ESP8266::cipSend){
			this->uartReceivedBuffer.clear();
		}
		delete this->state;
		this->state=nextState;
		nextState=nullptr;
		this->time=325000;
	}
	this->findIPDMessage();
	if(this->uartReceivedBuffer.size()>MAX_WIFI_RECEIVED_BUFFER){
		this->uartReceivedBuffer.erase(0,MAX_WIFI_RECEIVED_BUFFER/2);
		printf("uartReceivedBuffer wyczyszczony \r\n");
	}

}
int WifiESP8266ATCom::SendData(uint8_t *data,uint32_t size){
	if(this->state->readyToSend()){
		ESP8266CipSend *nextState=new ESP8266CipSend();
		nextState->sendData(data, size);
		this->uartReceivedBuffer.clear();
		this->state=nextState;
		this->time=0;
		//this->nextState=nextState;
		return 0;
	}
	return 1;
}
int WifiESP8266ATCom::SendCommand(uint8_t *data,uint32_t size){
	HAL_UART_Transmit_IT(this->uart, data, size);
	return 0;
}
int WifiESP8266ATCom::addUartData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		//this->uartReceivedBuffer.append(reinterpret_cast<char*>(data));
		bridgeReceivedBuffer.addData(data[i]);
	}
	return 0;
}
const char *  WifiESP8266ATCom::getUartData(int *size){
	*size=this->uartReceivedBuffer.size();
	if(*size>0){
		return this->uartReceivedBuffer.c_str();
	}else{
		return nullptr;
	}


}
void WifiESP8266ATCom::clearUartData(){
	this->uartReceivedBuffer.clear();
}
int WifiESP8266ATCom::GetResponse(std::string *data){
	if(this->ipdSize!=0 || this->ipdMessage.size()==0){
		findIPDMessage();
		if(this->ipdSize!=0 || this->ipdMessage.size()==0){
			return -1;
		}
	}

	data->append(this->ipdMessage);
	this->ipdMessage.clear();

	return 0;
}
void WifiESP8266ATCom::Reset(){
	HAL_GPIO_WritePin(resetGpio, resetPin, GPIO_PIN_RESET);
	wait(100);
	HAL_GPIO_WritePin(resetGpio, resetPin, GPIO_PIN_SET);
}
void WifiESP8266ATCom::TimeInterrupt(){
	this->time++;

}
void WifiESP8266ATCom::wait(uint32_t ms){
	this->time=0;
	while(this->time<ms){
	}
}
void WifiESP8266ATCom::ChangeState(ESP8266State *state){
}
void WifiESP8266ATCom::findIPDMessage(){
	char ipdStart[]="+IPD,";
	char ipdEnd[]=":";
	int ipdStartSize=sizeof(ipdStart)/sizeof(ipdStart[0])-1;
	int ipdEndSize=sizeof(ipdEnd)/sizeof(ipdEnd[0])-1;
	if(this->ipdSize>0){ //Pobierania do otrzymania całej ramki
		int bufferSize=this->uartReceivedBuffer.size();
		if(bufferSize==0){
			return;
		}
		int sizeToCopy=this->ipdSize;
		if(bufferSize<ipdSize){
			sizeToCopy=bufferSize;
		}
		this->ipdMessage.append(this->uartReceivedBuffer.substr(0,sizeToCopy));
		this->ipdSize-=sizeToCopy;
		this->uartReceivedBuffer.erase(0,sizeToCopy);
	}else{
		size_t ipdStartPosition=this->uartReceivedBuffer.find(ipdStart);
		size_t ipdEndPosition=std::string::npos;
		std::string numberToSTOIipdSize="";
		if(ipdStartPosition==std::string::npos){
			return;
		}
		//this->uartReceivedBuffer.erase(0,ipdStartPosition);
		ipdEndPosition=this->uartReceivedBuffer.find(ipdEnd);
		if(ipdEndPosition==std::string::npos){
			return;
		}
		numberToSTOIipdSize=this->uartReceivedBuffer.substr(ipdStartPosition+ipdStartSize,ipdEndPosition-(ipdStartPosition+ipdStartSize));
		for(size_t i=0;i<numberToSTOIipdSize.size();i++){
			if(numberToSTOIipdSize.at(i)<'0' ||numberToSTOIipdSize.at(i)>'9'){
				numberToSTOIipdSize.erase(i,1);
				i--;
			}
		}
		if(numberToSTOIipdSize.size()>0){
			this->ipdSize=stoi(numberToSTOIipdSize);
		}else{
			this->ipdSize=0;
		}
		this->uartReceivedBuffer.erase(0,ipdEndPosition+ipdEndSize);
		if(this->ipdMessage.size()>0){
			this->ipdMessage.clear();
		}
		if(this->ipdSize>0){
			this->findIPDMessage();
		}

	}
}
