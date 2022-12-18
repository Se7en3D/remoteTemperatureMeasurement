/*
 * ESP8266CommunicationTest.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266COMMUNICATIONTEST_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266COMMUNICATIONTEST_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266CommunicationTest: public virtual ESP8266State{
public:
	ESP8266CommunicationTest(WifiESP8266ATCom *parent);
	virtual ~ESP8266CommunicationTest();
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266COMMUNICATIONTEST_H_ */
