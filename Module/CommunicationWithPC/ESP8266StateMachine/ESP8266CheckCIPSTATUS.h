/*
 * ESP8266CheckCIPSTATUS.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>

class ESP8266CheckCIPSTATUS: public virtual ESP8266State{
public:
	ESP8266CheckCIPSTATUS();
	virtual ~ESP8266CheckCIPSTATUS();
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CHECKCIPSTATUS_H_ */
