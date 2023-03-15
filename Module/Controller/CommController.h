/*
 * SendController.h
 *
 *  Created on: 2 mar 2023
 *      Author: Daniel
 */

#ifndef CONTROLLER_COMMCONTROLLER_H_
#define CONTROLLER_COMMCONTROLLER_H_
#include "CommunicationWithPC/ComWithPC.h"
#include "CommunicationWithPC/Freames/FramesCreator.h"
#include "CommunicationWithPC/Freames/Decode/FramesDecoder.h"
#include <list>

class CommController {
public:
	CommController(ComWithPC *comWithPC,FramesCreator *framesCreator,FramesDecoder *framesDecoder,uint32_t maxTempBuffer);
	virtual ~CommController();
	bool addTempFrame(int *pTemperatures,uint32_t size);
	void main();

	void timerInterrupt(uint32_t time);
private:

	struct TempFrameBuffer{
		int *pTempValue=nullptr;
		uint32_t size;
		~TempFrameBuffer(){
			delete [] pTempValue;
		}
	};
	ComWithPC *comWithPC=nullptr;
	FramesCreator *framesCreator=nullptr;
	FramesDecoder *framesDecoder=nullptr;
	const uint32_t tempBufferMaxSize;
	std::list<TempFrameBuffer*> tempBuffer;
	int time;
};

#endif /* CONTROLLER_COMMCONTROLLER_H_ */
