/*
 * WifiESP8266ATCom.h
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_
#define COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_

#include <CommunicationWithPC/ComWithPC.h>
#include "usart.h"
#include "gpio.h"
#include <CommunicationWithPC/ESP8266StateMachine/ESP8266State.h>
#define WIFI_SET_MODE_COMMAND "AT+CWMODE=3\r\n"
#define WIFI_CONNECT_TO_ROUTE_COMMAND "AT+CWJAP=\"UPC1350937\",\"DNSAZUFX\"\r\n"
#define WIFI_CONNECT_TO_SERVER_COMMAND "AT+CIPSTART=\"TCP\",\"192.168.0.63\",4242\r\n"
#define WIFI_PASSTHROUGH_MODE_COMMAND "AT+CIPMODE=0\r\n"
#define WIFI_SEND_MODE_COMMAND "AT+CIPSEND\r\n"
#define WIFI_CHIPSTATUS_COMMAND "AT+CIPSTATUS\r\n"
#define WIFI_CHECK_CONNECTION_COMMAND "AT\r\n"

class WifiESP8266ATCom: public virtual ComWithPC {
public:
	WifiESP8266ATCom(UART_HandleTypeDef *uart,GPIO_TypeDef * resetGpio,uint16_t resetPin);
	virtual ~WifiESP8266ATCom();
	int Initialized() override;
	void Main() override;
	int SendData(uint8_t *data,uint32_t size) override;
	int GetFrameFromBuffer(uint8_t *data) override;
	int addData(uint8_t *data,uint32_t size) override;
	void Reset() override;
	void TimeInterrupt() override;
	void ChangeState(ESP8266State *state);
private:
	void wait(uint32_t ms);
	UART_HandleTypeDef *uart;
	GPIO_TypeDef * resetGpio;
	uint16_t resetPin;
	ESP8266State *state;
	uint32_t time;
};

#endif /* COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_ */
