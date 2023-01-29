/*
 * ESP8266CipSend.cpp
 *
 *  Created on: 1 sty 2023
 *      Author: Daniel
 */
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CipSend.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <cstring>
ESP8266CipSend::ESP8266CipSend(WifiESP8266ATCom *parent): ESP8266State(parent) {
	// TODO Auto-generated constructor stub

}

ESP8266CipSend::~ESP8266CipSend() {
	// TODO Auto-generated destructor stub
	if(this->data!=nullptr){
		delete[] this->data;
		this->data=nullptr;
	}
}
void ESP8266CipSend::setStatusToInicialized(){
	ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
	this->parent->ChangeState(nextState);
}
int ESP8266CipSend::initial(){
	static int falueInitialized=0;
	int dataSizeinBufferSender=0;
	switch(this->cipSendStatus){
	case sendDataStatus::noInitialized:
		dataSizeinBufferSender=sprintf(this->bufferSender,"AT+CIPSEND=%ld\r\n",this->dataSize);
		this->sendUartData(reinterpret_cast<uint8_t*>(bufferSender),dataSizeinBufferSender);
		this->resetTime();
		break;
	case sendDataStatus::Initialized:
		if(this->getTime()>10*COMMUNICATION_TEST_TIME_TO_REINIT){
			falueInitialized++;
			if(falueInitialized>MAX_CIPSEND_FAILURE_INITIALIZED){
				this->setStatusToInicialized();
			}
			this->resetTime();
		}
		break;
	case sendDataStatus::SendingData:
		if(this->getTime()>10*COMMUNICATION_TEST_TIME_TO_REINIT){
			this->sendUartData(reinterpret_cast<uint8_t*>(bufferSender),dataSizeinBufferSender);
			this->resetTime();
			this->setStatusToInicialized();
			this->parent->clearUartData();
		}
		break;
	}
	return -1;
}

void ESP8266CipSend::main(){
	if(this->data==nullptr || this->dataSize==0){
		ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
		this->parent->ChangeState(nextState);
	}
	if(this->parent==nullptr){
		return;
	}
	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
		initial();
	}
	const char *dataFromBuffer=nullptr;
	int bufferSize=0;
	dataFromBuffer=this->parent->getUartData(&bufferSize);
	if(strstr(dataFromBuffer,"ERROR")>0){
		ESP8266CheckCIPSTATUS *nextState=new ESP8266CheckCIPSTATUS(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		this->parent->clearUartData();
		return;
	}
	switch(this->cipSendStatus){
	case sendDataStatus::noInitialized:
		if(bufferSize>0 && dataFromBuffer!=nullptr){
			if(strstr(dataFromBuffer,"OK")>0){
				this->cipSendStatus=sendDataStatus::Initialized;
			}
		}
		break;
	case sendDataStatus::Initialized:
		if(strstr(dataFromBuffer,">")>0){
			this->sendUartData(reinterpret_cast<uint8_t*>(this->data),this->dataSize);
			this->cipSendStatus=sendDataStatus::SendingData;
			this->resetTime();
		}
	break;
	case sendDataStatus::SendingData:
		if(bufferSize>0 && dataFromBuffer!=nullptr){
			if(strstr(dataFromBuffer,"SEND OK")>0){
				this->setStatusToInicialized();
				this->parent->clearUartData();
			}
		}
	break;
	default:
		this->cipSendStatus=sendDataStatus::noInitialized;
		break;
	}
}
void ESP8266CipSend::sendData(const char* data,uint32_t size){
	if(this->data!=nullptr){
		delete[] this->data;
		this->data=nullptr;
	}
	this->data=new char[size];
	for(uint32_t i=0;i<size;i++){
		this->data[i]=data[i];
	}
	this->dataSize=size;
}
