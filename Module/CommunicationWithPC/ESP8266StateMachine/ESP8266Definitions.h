/*
 * ESP8266Definitions.h
 *
 *  Created on: 16 paź 2022
 *      Author: Daniel
 */
#pragma once
#ifndef COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266DEFINITIONS_H_
#define COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266DEFINITIONS_H_
#define PARENT_NO_SET_ERROR -1
#define COMMUNICATION_TEST_TIME_TO_REINIT 500 /*!<Czas do ponownego wysłania komendy w stanie testu komunikacji*/
#define MAX_ESP8266STATE_SIZE 100
#define MAX_CIPSEND_FAILURE_INITIALIZED 3


/**
 * Stałe teksty
 */
volatile static const uint8_t stringWifiSetMode[]="AT+CWMODE=3\r\n";
volatile static const uint8_t stringWifiConnectToRouter[]="AT+CWJAP=\"UPC1350937\",\"DNSAZUFX\"\r\n";
volatile static const uint8_t stringWifiConntectToServer[]="AT+CIPSTART=\"TCP\",\"192.168.0.72\",1024\r\n";
volatile static const uint8_t stringWifiModePassthrough[]="AT+CIPMODE=0\r\n";
volatile static const uint8_t stringWifiSendMode[]="AT+CIPSEND\r\n";
volatile static const uint8_t stringWifiCHIPSTATUS[]="AT+CIPSTATUS\r\n";
volatile static const uint8_t stringWifiCheckConnection[]="AT\r\n";


#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266DEFINITIONS_H_ */
