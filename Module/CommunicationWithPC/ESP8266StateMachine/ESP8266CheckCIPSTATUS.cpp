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
//#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
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

ESP8266State* ESP8266CheckCIPSTATUS::getNextState(std::string &buffer){
	if(buffer.find(this->dataToFind)==std::string::npos){
		return nullptr;
	}
	size_t signPosition=buffer.find(statusSigns);
	if(signPosition==std::string::npos){
		//Bufor wymaga wyczyszczenia
		return new ESP8266CheckCIPSTATUS();
	}
	signPosition+=(sizeof(statusSigns)/sizeof(statusSigns[0])-1);
	char tempChar=buffer.at(signPosition);
	switch(tempChar){
		// case '1':
		// 	return new ESP8266ConnectToRouter();
		// 	break;
		case '2':
			return new ESP8266ConnectToTCPServer();
			break;
		case '3':
			return new ESP8266Initialized();
			break;
		case '4':
			return new ESP8266ConnectToTCPServer();
			break;
		case '5':
			return new ESP8266SetMode();
			break;
		default:
			printf("CIPSTATUS nieznana wartość %d",static_cast<int>(tempChar));
			break;
	}
	return new ESP8266CheckCIPSTATUS();;
}
const uint8_t* ESP8266CheckCIPSTATUS::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiCHIPSTATUS)/sizeof(stringWifiCHIPSTATUS[0])-1;
	return (const uint8_t*)&stringWifiCHIPSTATUS[0];
}
bool ESP8266CheckCIPSTATUS::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}
inline ESP8266::stateName ESP8266CheckCIPSTATUS::getStateName(){
	return ESP8266::checkCIPSTATUS;
}
