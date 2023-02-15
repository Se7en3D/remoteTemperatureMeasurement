/*
 * WifiESP8266ATCom.h
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_
#define COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_

#include "CommunicationWithPC/ESP8266StateMachine/ESP8266State.h"
#include "CommunicationWithPC/ComWithPC.h"
#include "usart.h"
#include "gpio.h"
#include <string>
#define MAX_WIFI_RECEIVED_BUFFER 500
#define MAX_WIFI_TRANMITTER_BUFFER 500
class ESP8266State;

class WifiESP8266ATCom: public virtual ComWithPC {
public:
	WifiESP8266ATCom(UART_HandleTypeDef *uart,GPIO_TypeDef * resetGpio,uint16_t resetPin);
	virtual ~WifiESP8266ATCom();
	int Initialized() override;
	void Main() override;
	int SendData(uint8_t *data,uint32_t size) override;
	int SendCommand(uint8_t *data,uint32_t size);
	int GetResponse(std::string *data) override;
	int addUartData(uint8_t *data,uint32_t size) override;
	const char * getUartData(int *size);
	void clearUartData();
	void Reset() override;
	void TimeInterrupt() override;
	void ChangeState(ESP8266State *state);
private:
	UART_HandleTypeDef *uart;
	GPIO_TypeDef * resetGpio;
	uint16_t resetPin;
	ESP8266State *state;
	ESP8266State *nextState;
	uint32_t time;
	std::string uartReceivedBuffer;
	std::string uartTransmitterBuffer;
	int ipdSize=0;
	std::string ipdMessage="";
	void wait(uint32_t ms);
	void findIPDMessage();
};

#endif /* COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_ */
