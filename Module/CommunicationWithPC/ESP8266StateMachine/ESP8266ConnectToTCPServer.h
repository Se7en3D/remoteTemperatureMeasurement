/*
 * ESP8266ConnectToTCPServer.h
 *
 *  Created on: 15 paź 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include <vector>
class ESP8266ConnectToTCPServer: public virtual ESP8266State{
public:

	ESP8266ConnectToTCPServer(WifiESP8266ATCom *parent);
	virtual ~ESP8266ConnectToTCPServer();
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
};


#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CONNECTTOTCPSERVER_H_ */
