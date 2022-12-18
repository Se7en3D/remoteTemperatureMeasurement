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
#include <CommunicationWithPC/WifiESP8266ATCom.h>
#define PARENT_NO_SET_ERROR -1
#define COMMUNICATION_TEST_TIME_TO_REINIT 500 /*!<Czas do ponownego wysłania komendy w stanie testu komunikacji*/
#define MAX_ESP8266STATE_SIZE 100
class WifiESP8266ATCom;

class ESP8266State {
protected:
	inline static WifiESP8266ATCom *parent=nullptr;
private:
public:
	ESP8266State(WifiESP8266ATCom *parent);
	virtual ~ESP8266State();
	virtual int initial();
	virtual void main();
	virtual void timerInterrupt();
};



#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_ */
