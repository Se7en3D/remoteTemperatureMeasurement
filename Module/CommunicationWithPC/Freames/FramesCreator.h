/*
 * FramesCreator.h
 *
 *  Created on: 8 sty 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FRAMESCREATOR_H_
#define COMMUNICATIONWITHPC_FRAMESCREATOR_H_
#include <String>
class FramesCreator {
protected:
	std::string targetUrl;
public:
	FramesCreator();
	virtual ~FramesCreator();
	virtual std::string creatingTemperaturesTransmissionFrame(int* temperatures,uint32_t tempCount)=0;
	void setTargetUrl(char *targetUrl);
};

#endif /* COMMUNICATIONWITHPC_FRAMESCREATOR_H_ */
