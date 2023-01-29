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
ESP8266CommunicationTest::ESP8266CommunicationTest(WifiESP8266ATCom *parent): ESP8266State(parent){
}
ESP8266CommunicationTest::~ESP8266CommunicationTest(){
}
int ESP8266CommunicationTest::initial(){
	return this->sendUartData((uint8_t*)stringWifiCheckConnection, sizeof(stringWifiCheckConnection)-1);

}
void ESP8266CommunicationTest::main(){
	if(this->parent==nullptr){
		return;
	}
	if(this->getTime()>COMMUNICATION_TEST_TIME_TO_REINIT){
		this->initial();
	}

	char dataToFind[]="OK";
	const char *dataFromBuffer=nullptr;
	int bufferSize=0;
	dataFromBuffer=this->parent->getUartData(&bufferSize);

	if(bufferSize<=0 || dataFromBuffer==nullptr){
		return;
	}

	if(strstr((char*)dataFromBuffer,&dataToFind[0])==NULL){
		return;
	}
	this->parent->clearUartData();
	ESP8266CheckCIPSTATUS *nextState=new ESP8266CheckCIPSTATUS(ESP8266State::parent);
	this->parent->ChangeState(nextState);
}

