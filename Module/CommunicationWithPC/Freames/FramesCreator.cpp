/*
 * FramesCreator.cpp
 *
 *  Created on: 8 sty 2023
 *      Author: Daniel
 */

#include <CommunicationWithPC/Freames/FramesCreator.h>

FramesCreator::FramesCreator() {
	// TODO Auto-generated constructor stub

}

FramesCreator::~FramesCreator() {
	// TODO Auto-generated destructor stub
}
void FramesCreator::setTargetUrl(char *targetUrl){
	this->targetUrl.append(targetUrl);
}
