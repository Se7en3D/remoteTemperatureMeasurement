/*
 * FramesDecoderRest.h
 *
 *  Created on: 15 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_
#define COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_

#include <CommunicationWithPC/Freames/Decode/FramesDecoder.h>

class FramesDecoderRest: public FramesDecoder {
public:
	FramesDecoderRest();
	virtual ~FramesDecoderRest();
	DecodedFrame decodeFrame(std::string &frame) override;
};

#endif /* COMMUNICATIONWITHPC_FREAMES_FRAMESDECODERREST_H_ */
