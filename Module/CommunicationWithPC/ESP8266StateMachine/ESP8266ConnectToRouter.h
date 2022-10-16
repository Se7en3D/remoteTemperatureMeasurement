/*
 * ESP8266ConnectToRouter.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOROUTER_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOROUTER_H_
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
class ESP8266ConnectToRouter: public virtual ESP8266State{
public:
	ESP8266ConnectToRouter();
	virtual ~ESP8266ConnectToRouter();
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};
#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOROUTER_H_ */
