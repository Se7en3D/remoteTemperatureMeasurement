/*
 * ESP8266SetPassthroughMode.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266SetPassthroughMode: public virtual ESP8266State{
public:
	ESP8266SetPassthroughMode(WifiESP8266ATCom *parent);
	virtual ~ESP8266SetPassthroughMode();
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETPASSTHROUGHMODE_H_ */
