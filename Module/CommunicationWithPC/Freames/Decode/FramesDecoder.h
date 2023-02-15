/*
 * FramesDecoder.h
 *
 *  Created on: 11 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_
#define COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_

#include <string>
#include "GlobalData.h"
class FramesDecoder {
public:
	virtual DecodedFrame decodeFrame(std::string &frame)=0;
};

#endif /* COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_ */
