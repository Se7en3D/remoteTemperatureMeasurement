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

ESP8266State::ESP8266State() {

}

ESP8266State::~ESP8266State() {
	// TODO Auto-generated destructor stub
}

int ESP8266State::addData(uint8_t *data,uint32_t size){
	return -1;
}
int ESP8266State::initial(){
	return -1;
}
void ESP8266State::setParent(ComWithPC *parent){
	this->parent=parent;
}
void ESP8266State::main(){

}
void ESP8266State::timerInterrupt(){

}


