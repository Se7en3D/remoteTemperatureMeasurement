/*
 * ESP8266ConnectToTCPServer.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266SetPassthroughMode.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>




ESP8266ConnectToTCPServer::ESP8266ConnectToTCPServer(){
}
ESP8266ConnectToTCPServer::~ESP8266ConnectToTCPServer(){
}
ESP8266State* ESP8266ConnectToTCPServer::getNextState(std::string &buffer){
	if(buffer.find(this->dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266SetPassthroughMode();
}
const uint8_t* ESP8266ConnectToTCPServer::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiConntectToServer)/sizeof(stringWifiConntectToServer[0])-1;
	return (const uint8_t*)&stringWifiConntectToServer[0];
}
bool ESP8266ConnectToTCPServer::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT*4){
		return true;
	}else{
		return false;
	}
}
