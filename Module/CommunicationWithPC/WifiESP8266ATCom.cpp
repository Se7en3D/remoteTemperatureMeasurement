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
		this->nextState=nullptr;
	}
	if(this->state!=nullptr && this->nextState==nullptr){
		this->state->main();
	}
	//this->findIPDMessage();
	if(this->uartReceivedBuffer.size()>MAX_WIFI_RECEIVED_BUFFER){
		this->uartReceivedBuffer.erase(0,MAX_WIFI_RECEIVED_BUFFER/2);
	}

}
int WifiESP8266ATCom::SendData(uint8_t *data,uint32_t size){
	if(this->state->readyToSend() && nextState==nullptr){
		ESP8266CipSend *nextState=new ESP8266CipSend(this);
		nextState->sendData(reinterpret_cast<const char*>(data), size);
		//this->uartReceivedBuffer.clear();
		this->nextState=nextState;
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
		this->uartReceivedBuffer.append(reinterpret_cast<char*>(data));
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
		printf("STC=%d IZ=%d\r\n",sizeToCopy,this->ipdSize);
		if(this->ipdSize<0){
			printf("ipdSize=%d",this->ipdSize);
			//printf("%s\r\n",this->ipdMessage.c_str());
			//this->ipdMessage.clear();
		}
		this->uartReceivedBuffer.erase(0,sizeToCopy);
	}else{
		size_t ipdStartPosition=this->uartReceivedBuffer.find(ipdStart);
		size_t ipdEndPosition=std::string::npos;
		std::string numberToSTOIipdSize="";
		if(ipdStartPosition==std::string::npos){
			return;
		}
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
			printf("ipdSize=%d\r\n",this->ipdSize);
		//	this->findIPDMessage();
		}

	}
}
