/*
 * ESP8266CipSend.h
 *
 *  Created on: 1 sty 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CIPSEND_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CIPSEND_H_

#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
#include <string>
class ESP8266CipSend: public ESP8266State {
private:
	enum class sendDataStatus{
		noInitialized,
		Initialized,
		SendingData,
	};
	char bufferSender[30];
	char* data=nullptr;
	uint32_t dataSize=0;
	sendDataStatus cipSendStatus=sendDataStatus::noInitialized;
	void setStatusToInicialized();
public:
	ESP8266CipSend(WifiESP8266ATCom *parent);
	virtual ~ESP8266CipSend();
	int initial() override;
	void main() override;
	/**
	 * @brief Zapisanie danych do wysłania w prywatnych zmiennych
	 */
	void sendData(const char* data,uint32_t size);
};

#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CIPSEND_H_ */
