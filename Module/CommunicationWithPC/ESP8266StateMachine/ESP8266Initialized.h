/*
 * ESP8266Initialized.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266Initialized: public virtual ESP8266State{
public:
	ESP8266Initialized();
	virtual ~ESP8266Initialized();
	ESP8266State* getNextState(std::string &buffer) override;
	const uint8_t* getInitialData(uint32_t *size) override;
	bool readyToSendInit(int time) override;
	bool readyToSend() override;
	inline ESP8266::stateName getStateName() override;
private:
	static constexpr char dataClosed[]="CLOSED";
	static constexpr char dataWifiDisconnect[]="WIFI DISCONNECT";
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_ */
