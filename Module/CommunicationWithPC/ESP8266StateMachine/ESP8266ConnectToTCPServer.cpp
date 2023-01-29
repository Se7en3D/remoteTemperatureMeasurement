/*
 * ESP8266ConnectToTCPServer.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetPassthroughMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>




ESP8266ConnectToTCPServer::ESP8266ConnectToTCPServer(WifiESP8266ATCom *parent):ESP8266State(parent){
}
ESP8266ConnectToTCPServer::~ESP8266ConnectToTCPServer(){
}
int ESP8266ConnectToTCPServer::initial(){
	return this->sendUartData((uint8_t*)stringWifiConntectToServer, sizeof(stringWifiConntectToServer)-1);

}

void ESP8266ConnectToTCPServer::main(){
	if(this->parent==nullptr){
		return;
	}
	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT*4){ //Odczekaj 2 sek
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
	ESP8266SetPassthroughMode *nextState=new ESP8266SetPassthroughMode(ESP8266State::parent);
	this->parent->ChangeState(nextState);

}


