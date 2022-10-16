/*
 * ESP8266Initialized.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>


ESP8266Initialized::ESP8266Initialized(){
}
ESP8266Initialized::~ESP8266Initialized(){
}
int ESP8266Initialized::addData(uint8_t *data,uint32_t size){
/*	for(uint32_t i=0;i<size;i++){
		this->data.push_back(data[i]);
	}*/
	return 0;
}
int ESP8266Initialized::initial(){
/*	if(this->parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}

	this->time=0;
	this->data.clear();*/
	return  1;
}
void ESP8266Initialized::main(){
/*	if(this->parent==nullptr){
		return;
	}

	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	if(this->data.size()>MAX_ESP8266STATE_SIZE){
		this->data.clear();
	}*/
}
void ESP8266Initialized::timerInterrupt(){
	//this->time++;
}

