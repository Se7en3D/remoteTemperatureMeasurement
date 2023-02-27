/*
 * ESP8266SetPassthroughMode.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetPassthroughMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>


ESP8266SetPassthroughMode::ESP8266SetPassthroughMode(){
}
ESP8266SetPassthroughMode::~ESP8266SetPassthroughMode(){
}
//int ESP8266SetPassthroughMode::initial(){
//	return this->sendUartData((uint8_t*)stringWifiModePassthrough, sizeof(stringWifiModePassthrough)-1);
//
//}
//void ESP8266SetPassthroughMode::main(){
//	if(this->parent==nullptr){
//		return;
//	}
//	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
//		this->initial();
//	}
//
//	char dataToFind[]="OK";
//	const char *dataFromBuffer=nullptr;
//	int bufferSize=0;
//	dataFromBuffer=this->parent->getUartData(&bufferSize);
//
//	if(bufferSize<=0 || dataFromBuffer==nullptr){
//		return;
//	}
//
//	if(strstr((char*)dataFromBuffer,&dataToFind[0])==NULL){
//		return;
//	}
//	this->parent->clearUartData();
//	ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
//	this->parent->ChangeState(nextState);
//}
ESP8266State* ESP8266SetPassthroughMode::getNextState(std::string &buffer){
	if(buffer.find(dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266Initialized();
}
const uint8_t* ESP8266SetPassthroughMode::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiModePassthrough)/sizeof(stringWifiModePassthrough[0])-1;
	return (const uint8_t*)&stringWifiModePassthrough[0];
}
bool ESP8266SetPassthroughMode::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}
