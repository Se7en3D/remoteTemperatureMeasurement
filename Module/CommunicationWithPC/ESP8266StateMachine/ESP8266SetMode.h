/*
 * ESP8266SetMode.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETMODE_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETMODE_H_
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
class ESP8266SetMode: public virtual ESP8266State{
public:
	ESP8266SetMode();
	virtual ~ESP8266SetMode();
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266SETMODE_H_ */
