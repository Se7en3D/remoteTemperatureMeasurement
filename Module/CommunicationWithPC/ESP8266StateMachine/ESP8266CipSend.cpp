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
ESP8266CipSend::ESP8266CipSend() {
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
	//ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
	//this->parent->ChangeState(nextState);
}
//int ESP8266CipSend::initial(){
//	static int falueInitialized=0;
//	int dataSizeinBufferSender=0;
//	switch(this->cipSendStatus){
//	case sendDataStatus::noInitialized:
//		dataSizeinBufferSender=sprintf(this->bufferSender,"AT+CIPSEND=%ld\r\n",this->dataSize);
//		this->sendUartData(reinterpret_cast<uint8_t*>(bufferSender),dataSizeinBufferSender);
//		this->resetTime();
//		break;
//	case sendDataStatus::Initialized:
//		if(this->getTime()>10*COMMUNICATION_TEST_TIME_TO_REINIT){
//			falueInitialized++;
//			if(falueInitialized>MAX_CIPSEND_FAILURE_INITIALIZED){
//				this->setStatusToInicialized();
//			}
//			this->resetTime();
//		}
//		break;
//	case sendDataStatus::SendingData:
//		if(this->getTime()>10*COMMUNICATION_TEST_TIME_TO_REINIT){
//			this->parent->clearUartData();
//			this->sendUartData(reinterpret_cast<uint8_t*>(bufferSender),dataSizeinBufferSender);
//			this->resetTime();
//			this->setStatusToInicialized();
//		}
//		break;
//	}
//	return -1;
//}
//
//void ESP8266CipSend::main(){
//	if(this->data==nullptr || this->dataSize==0){
//		ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
//		this->parent->ChangeState(nextState);
//	}
//	if(this->parent==nullptr){
//		return;
//	}
//	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
//		initial();
//	}
//	const char *dataFromBuffer=nullptr;
//	int bufferSize=0;
//	dataFromBuffer=this->parent->getUartData(&bufferSize);
//	if(strstr(dataFromBuffer,"ERROR")>0){
//		ESP8266CheckCIPSTATUS *nextState=new ESP8266CheckCIPSTATUS(ESP8266State::parent);
//		this->parent->ChangeState(nextState);
//		this->parent->clearUartData();
//		return;
//	}
//	switch(this->cipSendStatus){
//	case sendDataStatus::noInitialized:
//		if(bufferSize>0 && dataFromBuffer!=nullptr){
//			if(strstr(dataFromBuffer,"OK")>0){
//				this->cipSendStatus=sendDataStatus::Initialized;
//			}
//		}
//		break;
//	case sendDataStatus::Initialized:
//		if(strstr(dataFromBuffer,">")>0){
//			this->sendUartData(reinterpret_cast<uint8_t*>(this->data),this->dataSize);
//			this->cipSendStatus=sendDataStatus::SendingData;
//			this->resetTime();
//		}
//	break;
//	case sendDataStatus::SendingData:
//		if(bufferSize>0 && dataFromBuffer!=nullptr){
//			if(strstr(dataFromBuffer,"SEND OK")>0){
//				this->setStatusToInicialized();
//			}
//		}
//	break;
//	default:
//		this->cipSendStatus=sendDataStatus::noInitialized;
//		break;
//	}
//}
void ESP8266CipSend::sendData(const uint8_t* data,uint32_t size){
	if(this->data!=nullptr){
		delete[] this->data;
		this->data=nullptr;
	}
	this->data=new uint8_t[size];
	memcpy(this->data,data,size);
	this->dataSize=size;
	this->changedState=true;
}
ESP8266State* ESP8266CipSend::getNextState(std::string &buffer){
	if(this->data==nullptr || this->dataSize==0){
		return new ESP8266Initialized();
	}
	if(buffer.find("ERROR")!=std::string::npos){
		return new ESP8266CheckCIPSTATUS();
	}
	switch(this->cipSendStatus){
	case sendDataStatus::noInitialized:
		if(buffer.find("OK")!=std::string::npos){
			this->cipSendStatus=sendDataStatus::Initialized;
			changedState=true;
		}
		break;
	case sendDataStatus::Initialized:
		if(buffer.find(">")!=std::string::npos){
			this->cipSendStatus=sendDataStatus::SendingData;
			changedState=true;
		}
	break;
	case sendDataStatus::SendingData:
		if(buffer.find("SEND OK")!=std::string::npos){
			return new ESP8266Initialized();
		}
	break;
	default:
		this->cipSendStatus=sendDataStatus::noInitialized;
		changedState=true;
		break;
	}
	return nullptr;
}
const uint8_t* ESP8266CipSend::getInitialData(uint32_t *size){
	static int falueInitialized=0;
	uint32_t dataSizeinBufferSender=0;
	switch(this->cipSendStatus){
	case sendDataStatus::noInitialized:
		dataSizeinBufferSender=sprintf(this->bufferSender,"AT+CIPSEND=%ld\r\n",this->dataSize);
		*size=dataSizeinBufferSender;
		return (uint8_t*) &this->bufferSender[0];
		break;
	case sendDataStatus::Initialized:
		if(this->tempTime>2*COMMUNICATION_TEST_TIME_TO_REINIT){
			falueInitialized++;
			if(falueInitialized>MAX_CIPSEND_FAILURE_INITIALIZED){
				this->setStatusToInicialized();
			}
		}
		break;
	case sendDataStatus::SendingData:
		*size=this->dataSize;
		return data;
		//this->sendUartData(reinterpret_cast<uint8_t*>(bufferSender),dataSizeinBufferSender);
		break;
	}
	return nullptr;
}
bool ESP8266CipSend::readyToSendInit(int time){
	this->tempTime=time;
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT*4 || changedState){
		changedState=false;
		return true;
	}else{
		return false;
	}
}
ESP8266::stateName ESP8266CipSend::getStateName(){
	return ESP8266::cipSend;
}
