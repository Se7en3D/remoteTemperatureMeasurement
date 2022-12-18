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
	if(ESP8266State::parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}
	this->parent->clearUartData();
	this->time=0;
	ESP8266State::parent->SendData((uint8_t*)stringWifiCHIPSTATUS, sizeof(stringWifiCHIPSTATUS)-1);

	return  1;
}
void ESP8266CheckCIPSTATUS::main(){
	//this->parent->ChangeState(new ESP8266ConnectToRouter);
	if(this->parent==nullptr){
		return;
	}
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}
	char dataToFind[]="OK";
	int bufferSize=0;
	char* dataFromBuffer=(char*)this->parent->getUartData(&bufferSize);

	if(bufferSize<=0 || dataFromBuffer==nullptr){
		return;
	}

	if(strstr(dataFromBuffer,&dataToFind[0])==NULL){
		return;
	}


	char *pointerToChar=strstr(dataFromBuffer,((char*)"STATUS:"));
	if(pointerToChar==NULL){
		this->parent->clearUartData();
		return;
	}
	pointerToChar=strstr(dataFromBuffer,((char*)":"));
	if(pointerToChar==NULL){
		this->parent->clearUartData();
		return;
	}
	pointerToChar++;
	switch(*pointerToChar){
	case '1':{
		ESP8266ConnectToRouter *nextState=new ESP8266ConnectToRouter(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		//ESP8266ConnectToRouter *newState=new ESP8266ConnectToRouter;
		//wifiParent->ChangeState(newState);
		//newState->setParent(this->parent);
		break;
	}
	case '2':{
		ESP8266ConnectToTCPServer *nextState=new ESP8266ConnectToTCPServer(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		//ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		return;
		break;
	}
	case '3':{
		ESP8266Initialized *nextState=new ESP8266Initialized(ESP8266State::parent);
		this->parent->ChangeState(nextState);
		//ESP8266Initialized *newState=new ESP8266Initialized;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		return;
		break;
	}
	case '4':{
		//ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		return;
		break;
	}
	case '5':{
		//ESP8266SetMode *newState=new ESP8266SetMode;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		return;
		break;
	}
	}
	this->parent->clearUartData();

}
void ESP8266CheckCIPSTATUS::timerInterrupt(){
	this->time++;
}
