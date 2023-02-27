/*
 * ESP8266ConnectToTCPServer.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266ConnectToTCPServer: public virtual ESP8266State{
public:

	ESP8266ConnectToTCPServer();
	virtual ~ESP8266ConnectToTCPServer();
	ESP8266State* getNextState(std::string &buffer) override;
	const uint8_t* getInitialData(uint32_t *size) override;
	bool readyToSendInit(int time) override;
private:
	static constexpr char dataToFind[]="OK";
};


#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_ */
