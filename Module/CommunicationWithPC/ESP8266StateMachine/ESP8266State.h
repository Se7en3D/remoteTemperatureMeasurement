/*
 * ESP8266State.h
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_
#include <stdio.h>
#include <vector>
#include <string>
#include "ESP8266Definitions.h"
#define PARENT_NO_SET_ERROR -1
#define COMMUNICATION_TEST_TIME_TO_REINIT 500 /*!<Czas do ponownego wysłania komendy w stanie testu komunikacji*/
#define MAX_ESP8266STATE_SIZE 100
class WifiESP8266ATCom;

class ESP8266State {
protected:
public:
	ESP8266State();
	virtual ~ESP8266State();
	virtual bool readyToSend();
	virtual ESP8266State* getNextState(std::string &buffer)=0;
	virtual const uint8_t* getInitialData(uint32_t *size)=0;
	virtual bool readyToSendInit(int time)=0;
	virtual inline ESP8266::stateName getStateName();
};



#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_ */
