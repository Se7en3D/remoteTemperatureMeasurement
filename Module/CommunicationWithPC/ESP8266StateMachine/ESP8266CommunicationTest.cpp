/*
 * ESP8266CommunicationTest.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CommunicationTest.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>

				/*ESP8266CommunicationTest*/
ESP8266CommunicationTest::ESP8266CommunicationTest(){
}
ESP8266CommunicationTest::~ESP8266CommunicationTest(){
}

ESP8266State* ESP8266CommunicationTest::getNextState(std::string &buffer){
	int bufferSize=buffer.size();
	if(bufferSize<=0){
		return nullptr;
	}
	if(buffer.find(dataToFind)==std::string::npos){
		return nullptr;
	}
	return new ESP8266CheckCIPSTATUS();
}
const uint8_t* ESP8266CommunicationTest::getInitialData(uint32_t *size){
	*size=sizeof(stringWifiCheckConnection)/sizeof(stringWifiCheckConnection[0])-1;
	return (const uint8_t*)&stringWifiCheckConnection[0];
}

bool ESP8266CommunicationTest::readyToSendInit(int time){
	if(time>COMMUNICATION_TEST_TIME_TO_REINIT){
		return true;
	}else{
		return false;
	}
}
