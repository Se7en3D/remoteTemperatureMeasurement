/*
 * ESP8266Initialized.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>


ESP8266Initialized::ESP8266Initialized(){
}
ESP8266Initialized::~ESP8266Initialized(){
}

ESP8266State* ESP8266Initialized::getNextState(std::string &buffer){
	if(buffer.find(dataClosed)!=std::string::npos || buffer.find(dataWifiDisconnect)!=std::string::npos ){
		return new ESP8266CheckCIPSTATUS();
	}
	return nullptr;
}
const uint8_t* ESP8266Initialized::getInitialData(uint32_t *size){
	*size=0;
	return nullptr;
}
bool ESP8266Initialized::readyToSendInit(int time){
	return false;
}
inline bool ESP8266Initialized::readyToSend(){
	return true;
}
ESP8266::stateName ESP8266Initialized::getStateName(){
	return ESP8266::initialized;
}