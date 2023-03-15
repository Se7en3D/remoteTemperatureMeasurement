/*
 * ESP8266SetPassthroughMode.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266SetPassthroughMode: public virtual ESP8266State{
public:
	ESP8266SetPassthroughMode();
	virtual ~ESP8266SetPassthroughMode();
	ESP8266State* getNextState(std::string &buffer) override;
	const uint8_t* getInitialData(uint32_t *size) override;
	bool readyToSendInit(int time) override;
private:
	static constexpr char dataToFind[]="OK";
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_ */
