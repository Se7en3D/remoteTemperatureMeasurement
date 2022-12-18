/*
 * ESP8266Initialized.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266Initialized: public virtual ESP8266State{
public:
	ESP8266Initialized(WifiESP8266ATCom *parent);
	virtual ~ESP8266Initialized();
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266INITIALIZED_H_ */
