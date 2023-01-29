/*
 * ESP8266SetMode.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

ESP8266SetMode::ESP8266SetMode(WifiESP8266ATCom *parent) : ESP8266State(parent){
}
ESP8266SetMode::~ESP8266SetMode(){
}
int ESP8266SetMode::initial(){
	return this->sendUartData((uint8_t*)stringWifiSetMode, sizeof(stringWifiSetMode)-1);
}

void ESP8266SetMode::main(){
	if(this->parent==nullptr){
		return;
	}
	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	char dataToFind[]="OK";
	const char *dataFromBuffer=nullptr;
	int bufferSize=0;
	dataFromBuffer=this->parent->getUartData(&bufferSize);

	if(bufferSize<=0 || dataFromBuffer==nullptr){
		return;
	}

	if(strstr((char*)dataFromBuffer,&dataToFind[0])==NULL){
		return;
	}
	this->parent->clearUartData();
	ESP8266ConnectToRouter *nextState=new ESP8266ConnectToRouter(ESP8266State::parent);
	this->parent->ChangeState(nextState);
}

