/*
 * ESP8266Initialized.cpp
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.h>
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstring>


ESP8266Initialized::ESP8266Initialized(WifiESP8266ATCom *parent): ESP8266State(parent){
}
ESP8266Initialized::~ESP8266Initialized(){
}

int ESP8266Initialized::initial(){
	return -1;
}

void ESP8266Initialized::main(){

}
void ESP8266Initialized::timerInterrupt(){

}

