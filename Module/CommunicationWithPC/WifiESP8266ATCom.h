/*
 * WifiESP8266ATCom.h
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_
#define COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_

#include <CommunicationWithPC/ComWithPC.h>

class WifiESP8266ATCom: public virtual ComWithPC {
public:
	WifiESP8266ATCom();
	virtual ~WifiESP8266ATCom();
};

#endif /* COMMUNICATIONWITHPC_WIFIESP8266ATCOM_H_ */
