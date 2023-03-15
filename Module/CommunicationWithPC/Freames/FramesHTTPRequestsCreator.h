/*
 * FramesHTTPRequestsCreator.h
 *
 *  Created on: 8 sty 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_FRAMESHTTPREQUESTSCREATOR_H_
#define COMMUNICATIONWITHPC_FREAMES_FRAMESHTTPREQUESTSCREATOR_H_

#include <CommunicationWithPC/Freames/FramesCreator.h>

class FramesHTTPRequestsCreator: public FramesCreator {
public:
	FramesHTTPRequestsCreator();
	virtual ~FramesHTTPRequestsCreator();
	std::string creatingTemperaturesTransmissionFrame(int* temperatures,uint32_t tempCount) override;
};

#endif /* COMMUNICATIONWITHPC_FREAMES_FRAMESHTTPREQUESTSCREATOR_H_ */
