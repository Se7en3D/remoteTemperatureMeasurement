/*
 * WiFi.h
 *
 *  Created on: 25 lip 2022
 *      Author: Daniel
 */

#ifndef INC_WIFI_H_
#define INC_WIFI_H_

typedef enum WiFiStates WiFiStates;
typedef enum WiFiEvents WiFiEvents;
typedef enum WiFiErrorCode WiFiErrorCode;
typedef struct WiFiClass WiFiClass;
/**
 * @brief Different state of State Machine
 */
enum WiFiStates{
	WiFiStateIdle,
	WiFiStateCheckCIPSTATUS,
	WiFiStateSetMode,
	WiFiStateConnectToRouter,
	WiFiStateConnectToTCPServer,
	WiFiStateSetPassthroughMode,
	WiFiStateInitialized,
	WiFiStateStartSendMode,
	WiFiStateStopSendMode,
};
/**
 * @breif Different event of State Machine
 */
enum  WiFiEvents{
	WiFiEventIdle,
	WiFIEventEntry,
	WiFiEventTimeout,
	WiFiEventCommandAnswer,
	WiFiEventCorrectResponse,
	WiFiEventIncorrectResponse,
	WiFiEventWaitToResponse,
	WiFiEventSendModeMarker
};
enum WiFiErrorCode{
	WifiNoError,
	WifiCWJAPConnectionTimeout,
	WifiCWJAPWrongPassword,
	WifiCWJAPCannotFindTheTargetAP,
	wifiCWJAPConnectionFailed,
};
/**
 * Struct for wifi state machine
 */
struct WiFiClass{
	WiFiStates eStateMachine;
	WiFiEvents eStateMachineEvent;
	WiFiErrorCode errorCode;
	uint32_t sysTick;
	uint32_t commandInBuffor;
	WiFiEvents (*getState)(WiFiClass *);
	void (*increaseSysTick)(WiFiClass *);
	void (*sendData)(WiFiClass *,uint8_t*,uint32_t);
	CircularBufferStruct *RxBuffer;
	CircularBufferStruct *TxBuffer;
	uint32_t txDataSize;
};
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
volatile static const uint8_t sprintfBuffer[100];
volatile static UART_HandleTypeDef *WiFiHuart=NULL;
uint8_t Rx_data[50];

WiFiClass *wiFiCreateClass(UART_HandleTypeDef *huart);

void setWiFiFunction(WiFiClass  *this,
		WiFiEvents (*getState)(WiFiClass *),
		void (*increaseTimer)(WiFiClass *),
		void (*sendData)(WiFiClass *,uint8_t*,uint32_t)
		);
void WiFiMainFunction(WiFiClass  *this);

WiFiStates WiFiStateFunctionIdle(WiFiClass *this);
WiFiStates WiFiCheckCHIPSTATUS(WiFiClass *this);
WiFiStates WiFiStateFunctionSetMode(WiFiClass *this);
WiFiStates WiFiStateFunctionConnectToRouter(WiFiClass *this);
WiFiStates WiFiStateFunctionConnectToTCPServer(WiFiClass *this);
WiFiStates WiFiStateFunctionSetPassthroughMode(WiFiClass *this);
WiFiStates WiFiStateFunctionInitialized(WiFiClass *this);
WiFiStates WiFiStateFunctionStartSendMode(WiFiClass *this);
WiFiStates WiFiStateFunctionStopSendMode(WiFiClass *this);

WiFiEvents WifiGetState(WiFiClass *this);
void WiFiSendData(WiFiClass *this,uint8_t* data,uint32_t size);
uint8_t* WiFiGetCommand(WiFiClass *this);
void WiFiSendCommand(const volatile uint8_t *pData, uint16_t Size);
void WiFiAddDataFromUART(WiFiClass *this,const volatile uint8_t *pData);
void WiFiIncreaseSysTick(WiFiClass *this);
void WiFiClearCommandBuffor(WiFiClass *this);
void WiFiDecodeErrorCode(WiFiClass *this,uint8_t *code);
uint8_t WiFiIsInicialized(WiFiClass *this);
volatile static const WiFiStates (*WiFiStateFunctionArrayWiFiEvents[])(WiFiClass *)={
		WiFiStateFunctionIdle,
		WiFiCheckCHIPSTATUS,
		WiFiStateFunctionSetMode,
		WiFiStateFunctionConnectToRouter,
		WiFiStateFunctionConnectToTCPServer,
		WiFiStateFunctionSetPassthroughMode,
		WiFiStateFunctionInitialized,
		WiFiStateFunctionStartSendMode,
		WiFiStateFunctionStopSendMode
};
#endif /* INC_WIFI_H_ */
