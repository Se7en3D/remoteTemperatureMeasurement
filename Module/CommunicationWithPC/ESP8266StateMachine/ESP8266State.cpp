/*
 * ESP8266State.cpp
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>
ESP8266State::ESP8266State() {

}

ESP8266State::~ESP8266State() {
	// TODO Auto-generated destructor stub
}

int ESP8266State::addData(uint8_t *data,uint32_t size){
	return -1;
}
int ESP8266State::initial(){
	return -1;
}
void ESP8266State::setParent(ComWithPC *parent){

}
void ESP8266State::main(){

}
void ESP8266State::timerInterrupt(){

}


				/*ESP8266CommunicationTest*/
ESP8266CommunicationTest::ESP8266CommunicationTest(){
	this->parent=nullptr;
	this->time=0;
}
ESP8266CommunicationTest::~ESP8266CommunicationTest(){
	this->data.clear();
}
void ESP8266CommunicationTest::setParent(ComWithPC *parent){
	this->parent=parent;
}
int ESP8266CommunicationTest::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266CommunicationTest::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiCheckConnection, sizeof(stringWifiCheckConnection)-1);
	return  1;
}
void ESP8266CommunicationTest::main(){
	if(this->parent==nullptr){
		return;
	}
	char dataToFind[]="OK";
	char* dataFromBuffer=static_cast<char*>(this->data.data());

	if(strstr(dataFromBuffer,&dataToFind[0])!=NULL){
		//Przejście do nastepnego stanu
		WifiESP8266ATCom *wifiParent=reinterpret_cast<WifiESP8266ATCom*>(this->parent);
		if(wifiParent!=nullptr){
			ESP8266CheckCIPSTATUS *newState=new ESP8266CheckCIPSTATUS;
			wifiParent->ChangeState(newState);
			newState->setParent(this->parent);
			newState->initial();
		}else{ //TODO Przemyśleć co zrobić jak nie będzie castu
			this->initial();
		}
		this->data.clear();
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}
	if((this->data.size()) > MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266CommunicationTest::timerInterrupt(){
	this->time++;
}
				/*ESP8266CheckCIPSTATUS*/
ESP8266CheckCIPSTATUS::ESP8266CheckCIPSTATUS(){
	this->parent=nullptr;
}
ESP8266CheckCIPSTATUS::~ESP8266CheckCIPSTATUS(){
	this->data.clear();
}
void ESP8266CheckCIPSTATUS::setParent(ComWithPC *parent){
	this->parent=parent;
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
		if(pointerToChar!=NULL){
			pointerToChar=strstr(dataFromBuffer,((char*)":"));
			pointerToChar++;
			switch(*pointerToChar){
			case '1':{
				ESP8266ConnectToRouter *newState=new ESP8266ConnectToRouter;
				wifiParent->ChangeState(newState);
				newState->setParent(this->parent);
				break;
			}
			case '2':{
				ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
				wifiParent->ChangeState(newState);
				newState->setParent(this->parent);
				break;
			}
			case '3':{
				ESP8266Initialized *newState=new ESP8266Initialized;
				wifiParent->ChangeState(newState);
				newState->setParent(this->parent);
				break;
			}
			case '4':{
				ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
				wifiParent->ChangeState(newState);
				newState->setParent(this->parent);
				break;
			}
			case '5':{
				ESP8266SetMode *newState=new ESP8266SetMode;
				wifiParent->ChangeState(newState);
				newState->setParent(this->parent);
				break;
			}
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

			/*ESP8266SetMode*/
ESP8266SetMode::ESP8266SetMode(){
	this->parent=nullptr;
}
ESP8266SetMode::~ESP8266SetMode(){
	this->data.clear();
}
void ESP8266SetMode::setParent(ComWithPC *parent){
	this->parent=parent;
}
int ESP8266SetMode::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266SetMode::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiSetMode, sizeof(stringWifiSetMode)-1);
	return  1;
}
void ESP8266SetMode::main(){
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
		ESP8266ConnectToRouter *newState=new ESP8266ConnectToRouter;
		wifiParent->ChangeState(newState);
		newState->setParent(this->parent);
		this->data.clear();
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266SetMode::timerInterrupt(){
	this->time++;
}



			/*ESP8266ConnectToRouter*/
ESP8266ConnectToRouter::ESP8266ConnectToRouter(){
	this->parent=nullptr;
}
ESP8266ConnectToRouter::~ESP8266ConnectToRouter(){
	this->data.clear();
}
void ESP8266ConnectToRouter::setParent(ComWithPC *parent){
	this->parent=parent;
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
		ESP8266ConnectToTCPServer *newState=new ESP8266ConnectToTCPServer;
		wifiParent->ChangeState(newState);
		newState->setParent(this->parent);
		this->data.clear();
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



			/*ESP8266ConnectToTCPServer*/
ESP8266ConnectToTCPServer::ESP8266ConnectToTCPServer(){
	this->parent=nullptr;
}
ESP8266ConnectToTCPServer::~ESP8266ConnectToTCPServer(){
	this->data.clear();
}
void ESP8266ConnectToTCPServer::setParent(ComWithPC *parent){
	this->parent=parent;
}
int ESP8266ConnectToTCPServer::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266ConnectToTCPServer::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiConntectToServer, sizeof(stringWifiConntectToServer)-1);
	return  1;
}
void ESP8266ConnectToTCPServer::main(){
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
		ESP8266SetPassthroughMode *newState=new ESP8266SetPassthroughMode;
		wifiParent->ChangeState(newState);
		newState->setParent(this->parent);
		this->data.clear();
	}
	if(strstr(dataFromBuffer,((char*)"ALREADY CONNECTED"))){
		WifiESP8266ATCom *wifiParent=reinterpret_cast<WifiESP8266ATCom*>(this->parent);
		if(wifiParent==nullptr){
			this->data.clear();
		}
		ESP8266Initialized *newState=new ESP8266Initialized;
		wifiParent->ChangeState(newState);
		newState->setParent(this->parent);
		this->data.clear();
	}
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266ConnectToTCPServer::timerInterrupt(){
	this->time++;
}

		/*ESP8266SetPassthroughMode*/
ESP8266SetPassthroughMode::ESP8266SetPassthroughMode(){
	this->parent=nullptr;
}
ESP8266SetPassthroughMode::~ESP8266SetPassthroughMode(){
	this->data.clear();
}
void ESP8266SetPassthroughMode::setParent(ComWithPC *parent){
	this->parent=parent;
}
int ESP8266SetPassthroughMode::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266SetPassthroughMode::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	this->parent->SendData((uint8_t*)stringWifiModePassthrough, sizeof(stringWifiModePassthrough)-1);
	return  1;
}
void ESP8266SetPassthroughMode::main(){
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
		ESP8266Initialized *newState=new ESP8266Initialized;
		wifiParent->ChangeState(newState);
		newState->setParent(this->parent);
		this->data.clear();
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266SetPassthroughMode::timerInterrupt(){
	this->time++;
}


		/*ESP8266Initialized*/
ESP8266Initialized::ESP8266Initialized(){
	this->parent=nullptr;
}
ESP8266Initialized::~ESP8266Initialized(){
	this->data.clear();
}
void ESP8266Initialized::setParent(ComWithPC *parent){
	this->parent=parent;
}
int ESP8266Initialized::addData(uint8_t *data,uint32_t size){
	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}
	return 0;
}
int ESP8266Initialized::initial(){
	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();
	return  1;
}
void ESP8266Initialized::main(){
	if(this->parent==nullptr){
		return;
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}
}
void ESP8266Initialized::timerInterrupt(){
	this->time++;
}
