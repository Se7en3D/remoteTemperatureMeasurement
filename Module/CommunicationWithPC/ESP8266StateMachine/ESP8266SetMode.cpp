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
	if(ESP8266State::parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}
	this->parent->clearUartData();
	this->time=0;
	ESP8266State::parent->SendData((uint8_t*)stringWifiSetMode, sizeof(stringWifiSetMode)-1);
	return  1;
}

void ESP8266SetMode::main(){
	if(this->parent==nullptr){
		return;
	}
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	char dataToFind[]="OK";
	uint8_t *dataFromBuffer=nullptr;
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
void ESP8266SetMode::timerInterrupt(){
	this->time++;
}
