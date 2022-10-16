/*
 * ESP8266ConnectToRouter.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

ESP8266ConnectToRouter::ESP8266ConnectToRouter(){
}
ESP8266ConnectToRouter::~ESP8266ConnectToRouter(){
}
int ESP8266ConnectToRouter::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266ConnectToRouter::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiConnectToRouter, sizeof(stringWifiConnectToRouter)-1);
	return  1;
}
void ESP8266ConnectToRouter::main(){
	if(this->parent==nullptr){
		return;
	}
	char dataToFind[]="OK";
	char* dataFromBuffer=static_cast<char*>(this->data.data());

	if(strstr(dataFromBuffer,&dataToFind[0])!=NULL){
		//Przejście do nastepnego stanu
		WifiESP8266ATCom *wifiParent=reinterpret_cast<WifiESP8266ATCom*>(this->parent);
		if(wifiParent==nullptr){
			this->data.clear();
		}
		//ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		return;
	}
	if(strstr(dataFromBuffer,(char*)"ERROR")!=NULL){
		this->data.clear(); //TODO dodać obsługę gdy jest to zła komenda
	}

	if(strstr(dataFromBuffer,(char*)":")!=NULL){
		this->data.clear(); //TODO dodać obsługę gdy jest błąd z połaczeniem
	}
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266ConnectToRouter::timerInterrupt(){
	this->time++;
}

