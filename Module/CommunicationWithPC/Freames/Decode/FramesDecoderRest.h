/*
 * FramesDecoderRest.h
 *
 *  Created on: 15 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_
#define COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_

#include <CommunicationWithPC/Freames/Decode/FramesDecoder.h>
#include "HttpMessage.h"
#include "../ContentType/ContentTypeGetter.h"
class FramesDecoderRest: public FramesDecoder {
public:
	FramesDecoderRest();
	virtual ~FramesDecoderRest();
	int decodeFrame(std::string &inputFrame,DecodedFrame* decodedFrame) override;
private:
	ContentTypeGetter *contentTypeDecoder=nullptr;
	HttpMessage httpMessage;
	decodedFrameType getMessageType(HttpFrame *httpFrame);
	int checkContentType(HttpFrame *httpFrame);
	int decodeSetTime(HttpFrame *httpFrame);

};

#endif /* COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_ */
