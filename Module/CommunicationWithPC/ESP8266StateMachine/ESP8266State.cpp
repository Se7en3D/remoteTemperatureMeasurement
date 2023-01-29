/*
 * ESP8266State.cpp
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>
ESP8266State::ESP8266State(WifiESP8266ATCom *parent) {
	//ESP8266State::parent=parent;
	this->parent=parent;
	this->time=COMMUNICATION_TEST_TIME_TO_REINIT*10;

}

ESP8266State::~ESP8266State() {
	// TODO Auto-generated destructor stub
}

void ESP8266State::timerInterrupt(){
	this->time++;
}

int ESP8266State::sendUartData(uint8_t *data,uint32_t size){
	if(ESP8266State::parent==nullptr){
		return PARENT_NO_SET_ERROR;
	}
	this->parent->clearUartData();
	this->time=0;
	this->parent->SendCommand(data, size);
	return  1;
}
inline bool ESP8266State::readyToSend(){
	return false;
}
