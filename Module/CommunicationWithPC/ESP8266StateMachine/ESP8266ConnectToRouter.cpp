/*
 * ESP8266ConnectToRouter.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

ESP8266ConnectToRouter::ESP8266ConnectToRouter(WifiESP8266ATCom *parent): ESP8266State(parent){
}
ESP8266ConnectToRouter::~ESP8266ConnectToRouter(){
}
int ESP8266ConnectToRouter::initial(){
	if(ESP8266State::parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}
	this->parent->clearUartData();
	this->time=0;
	ESP8266State::parent->SendData((uint8_t*)stringWifiConnectToRouter, sizeof(stringWifiConnectToRouter)-1);
	return  1;
}

void ESP8266ConnectToRouter::main(){
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
	ESP8266ConnectToTCPServer *nextState=new ESP8266ConnectToTCPServer(ESP8266State::parent);
	this->parent->ChangeState(nextState);
}
void ESP8266ConnectToRouter::timerInterrupt(){
	this->time++;
}

