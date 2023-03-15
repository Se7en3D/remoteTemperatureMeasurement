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
public:
	ESP8266CipSend();
	virtual ~ESP8266CipSend();
	ESP8266State* getNextState(std::string &buffer) override;
	const uint8_t* getInitialData(uint32_t *size) override;
	bool readyToSendInit(int time) override;
	inline ESP8266::stateName getStateName() override;
	/**
	 * @brief Zapisanie danych do wysłania w prywatnych zmiennych
	 */
	void sendData(const uint8_t* data,uint32_t size);


private:
	enum class sendDataStatus{
		noInitialized,
		Initialized,
		SendingData,
	};
	bool changedState=false;
	char bufferSender[30];
	uint8_t* data=nullptr;
	uint32_t dataSize=0;
	int tempTime=0;
	sendDataStatus cipSendStatus=sendDataStatus::noInitialized;
	void setStatusToInicialized();

};


#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266CIPSEND_H_ */
