/*
 * ESP8266SetMode.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

ESP8266SetMode::ESP8266SetMode(){
}
ESP8266SetMode::~ESP8266SetMode(){
}
ESP8266State* ESP8266SetMode::getNextState(std::string &buffer){
	if(buffer.find(dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266ConnectToRouter();
}
const uint8_t* ESP8266SetMode::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiSetMode)/sizeof(stringWifiSetMode[0])-1;
	return (const uint8_t*)&stringWifiSetMode[0];
}
bool ESP8266SetMode::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}

ESP8266::stateName ESP8266SetMode::getStateName(){
	return ESP8266::setMode;
}