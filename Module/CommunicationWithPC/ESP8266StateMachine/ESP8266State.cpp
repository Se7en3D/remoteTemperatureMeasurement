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

inline bool ESP8266State::readyToSend(){
	return false;
}
ESP8266::stateName ESP8266State::getStateName(){
	return ESP8266::noState;
}
