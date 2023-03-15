/*
 * ESP8266SetPassthroughMode.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetPassthroughMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>


ESP8266SetPassthroughMode::ESP8266SetPassthroughMode(){
}
ESP8266SetPassthroughMode::~ESP8266SetPassthroughMode(){
}
ESP8266State* ESP8266SetPassthroughMode::getNextState(std::string &buffer){
	if(buffer.find(dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266Initialized();
}
const uint8_t* ESP8266SetPassthroughMode::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiModePassthrough)/sizeof(stringWifiModePassthrough[0])-1;
	return (const uint8_t*)&stringWifiModePassthrough[0];
}
bool ESP8266SetPassthroughMode::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}
ESP8266::stateName ESP8266SetPassthroughMode::getStateName(){
	return ESP8266::setPasshroughMode;
}