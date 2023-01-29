/*
 * ESP8266CheckCIPSTATUS.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266CheckCIPSTATUS: public virtual ESP8266State{
public:
	ESP8266CheckCIPSTATUS(WifiESP8266ATCom *parent);
	virtual ~ESP8266CheckCIPSTATUS();
	int initial() override;
	void main() override;
private:
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_ */
