/*
 * ESP8266CheckCIPSTATUS.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>
				/*ESP8266CheckCIPSTATUS*/
ESP8266CheckCIPSTATUS::ESP8266CheckCIPSTATUS(WifiESP8266ATCom *parent): ESP8266State(parent){
}
ESP8266CheckCIPSTATUS::~ESP8266CheckCIPSTATUS(){
}


int ESP8266CheckCIPSTATUS::initial(){
	return this->sendUartData((uint8_t*)stringWifiCHIPSTATUS, sizeof(stringWifiCHIPSTATUS)-1);
}
void ESP8266CheckCIPSTATUS::main(){
	if(this->parent==nullptr){
		return;
	}
	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}
	char dataToFind[]="OK";
	char statusSigns[]="STATUS:";
	int bufferSize=0;
	char* dataFromBuffer=(char*)this->parent->getUartData(&bufferSize);

	if(bufferSize<=0 || dataFromBuffer==nullptr){
		return;
	}

	if(strstr(dataFromBuffer,&dataToFind[0])==NULL){
		return;
	}


	char *pointerToChar=strstr(dataFromBuffer,statusSigns);
	if(pointerToChar==NULL){
		this->parent->clearUartData();
		return;
	}
	pointerToChar+=(sizeof(statusSigns)/sizeof(char)-1);
	char dataToSwitch=*pointerToChar;
	this->parent->clearUartData();
	switch(dataToSwitch){
	case '1':{
		ESP8266ConnectToRouter *nextState=new ESP8266ConnectToRouter(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		break;
	}
	case '2':{
		ESP8266ConnectToTCPServer *nextState=new ESP8266ConnectToTCPServer(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		return;
		break;
	}
	case '3':{
		ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		return;
		break;
	}
	case '4':{
		ESP8266ConnectToTCPServer *nextState=new ESP8266ConnectToTCPServer(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		return;
		break;
	}
	case '5':{
		ESP8266SetMode *nextState=new ESP8266SetMode(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		return;
		break;
	}
	}

}

