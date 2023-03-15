/*
 * ESP8266ConnectToRouter.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

ESP8266ConnectToRouter::ESP8266ConnectToRouter(){
}
ESP8266ConnectToRouter::~ESP8266ConnectToRouter(){
}
ESP8266State* ESP8266ConnectToRouter::getNextState(std::string &buffer) {
	if(buffer.find(this->dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266ConnectToTCPServer();
}
const uint8_t* ESP8266ConnectToRouter::getInitialData(uint32_t *size) {
	*size=sizeof(stringWifiConnectToRouter)/sizeof(stringWifiConnectToRouter)-1;
	return (const uint8_t*)&stringWifiConnectToRouter[0];
}
bool ESP8266ConnectToRouter::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}
ESP8266::stateName ESP8266ConnectToRouter::getStateName(){
	return ESP8266::connectToRouter;
}
