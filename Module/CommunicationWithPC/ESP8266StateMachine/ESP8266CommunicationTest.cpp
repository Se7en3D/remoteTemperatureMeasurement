/*
 * ESP8266CommunicationTest.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CommunicationTest.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

				/*ESP8266CommunicationTest*/
ESP8266CommunicationTest::ESP8266CommunicationTest(){
	this->time=0;
}
ESP8266CommunicationTest::~ESP8266CommunicationTest(){
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
			return;
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
