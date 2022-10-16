/*
 * ESP8266State.h
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_
#include "CommunicationWithPC/ComWithPC.h"
#include <vector>
#define PARENT_NO_SET_ERROR -1
#define COMMUNICATION_TEST_TIME_TO_REINIT 500 /*!<Czas do ponownego wysłania komendy w stanie testu komunikacji*/
#define MAX_ESP8266STATE_SIZE 100
/**
 * Stałe teksty
 */
volatile static const uint8_t stringWifiSetMode[]="AT+CWMODE=3\r\n";
volatile static const uint8_t stringWifiConnectToRouter[]="AT+CWJAP=\"UPC1350937\",\"DNSAZUFX\"\r\n";
volatile static const uint8_t stringWifiConntectToServer[]="AT+CIPSTART=\"TCP\",\"192.168.0.63\",4242\r\n";
volatile static const uint8_t stringWifiModePassthrough[]="AT+CIPMODE=0\r\n";
volatile static const uint8_t stringWifiSendMode[]="AT+CIPSEND\r\n";
volatile static const uint8_t stringWifiCHIPSTATUS[]="AT+CIPSTATUS\r\n";
volatile static const uint8_t stringWifiCheckConnection[]="AT\r\n";


class ESP8266State {
protected:
	ComWithPC *parent;
private:
public:
	ESP8266State();
	virtual ~ESP8266State();
	void setParent(ComWithPC *parent);
	virtual int addData(uint8_t *data,uint32_t size);
	virtual int initial();
	virtual void main();
	virtual void timerInterrupt();
};



#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_ */
