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
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>
				/*ESP8266CheckCIPSTATUS*/
ESP8266CheckCIPSTATUS::ESP8266CheckCIPSTATUS(){
}
ESP8266CheckCIPSTATUS::~ESP8266CheckCIPSTATUS(){
}

int ESP8266CheckCIPSTATUS::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266CheckCIPSTATUS::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiCHIPSTATUS, sizeof(stringWifiCHIPSTATUS)-1);
	return  1;
}
void ESP8266CheckCIPSTATUS::main(){
	if(this->parent==nullptr){
		return;
	}
	char dataToFind[]="OK";
	char* dataFromBuffer=static_cast<char*>(this->data.data());

	if(strstr(dataFromBuffer,&dataToFind[0])!=NULL){
		//Przejście do nastepnego stanu
		WifiESP8266ATCom *wifiParent=reinterpret_cast<WifiESP8266ATCom*>(this->parent);
		if(wifiParent==nullptr){
			return;
		}
		char *pointerToChar=strstr(dataFromBuffer,((char*)"STATUS:"));
		if(pointerToChar==NULL){
			this->data.clear();
			return;
		}

		pointerToChar=strstr(dataFromBuffer,((char*)":"));
		if(pointerToChar==NULL){
			this->data.clear();
			return;
		}
		pointerToChar++;

		//ESP8266ConnectToRouter *newState=new ESP8266ConnectToRouter;
		//newState->setParent(this->parent);
		//wifiParent->ChangeState(newState);
		//return;
			switch(*pointerToChar){
			case '1':{
				ESP8266ConnectToRouter *newState=new ESP8266ConnectToRouter;
				wifiParent->ChangeState(newState);
				//newState->setParent(this->parent);
				break;
			}
			case '2':{
				ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
				//newState->setParent(this->parent);
				wifiParent->ChangeState(newState);
				return;
				break;
			}
			case '3':{
				ESP8266Initialized *newState=new ESP8266Initialized;
				//newState->setParent(this->parent);
				wifiParent->ChangeState(newState);
				return;
				break;
			}
			case '4':{
				ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
				//newState->setParent(this->parent);
				wifiParent->ChangeState(newState);
				return;
				break;
			}
			case '5':{
				ESP8266SetMode *newState=new ESP8266SetMode;
				//newState->setParent(this->parent);
				wifiParent->ChangeState(newState);
				return;
				break;
			}
			}
		this->data.clear();
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266CheckCIPSTATUS::timerInterrupt(){
	this->time++;
}
