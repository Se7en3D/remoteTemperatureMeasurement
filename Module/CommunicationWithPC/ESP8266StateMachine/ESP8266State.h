/*
 * ESP8266State.h
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel
 */

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
private:
public:
	ESP8266State();
	virtual ~ESP8266State();
	virtual void setParent(ComWithPC *parent);
	virtual int addData(uint8_t *data,uint32_t size);
	virtual int initial();
	virtual void main();
	virtual void timerInterrupt();
};



class ESP8266CommunicationTest: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266CommunicationTest();
	virtual ~ESP8266CommunicationTest();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266CheckCIPSTATUS: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266CheckCIPSTATUS();
	virtual ~ESP8266CheckCIPSTATUS();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266SetMode: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266SetMode();
	virtual ~ESP8266SetMode();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266ConnectToRouter: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266ConnectToRouter();
	virtual ~ESP8266ConnectToRouter();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266ConnectToTCPServer: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266ConnectToTCPServer();
	virtual ~ESP8266ConnectToTCPServer();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266SetPassthroughMode: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266SetPassthroughMode();
	virtual ~ESP8266SetPassthroughMode();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};

class ESP8266Initialized: public virtual ESP8266State{
public:
	ComWithPC *parent;
	ESP8266Initialized();
	virtual ~ESP8266Initialized();
	void setParent(ComWithPC *parent) override;
	int addData(uint8_t *data,uint32_t size) override;
	int initial() override;
	void main() override;
	void timerInterrupt() override;
private:
	uint32_t time;
	std::vector<char> data;
};
#endif /* COMMUNICATIONWITHPC_ESP8266STATEMACHINE_ESP8266STATE_H_ */
