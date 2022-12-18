/*
 * ComWithPC.cpp
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#include <CommunicationWithPC/ComWithPC.h>

ComWithPC::ComWithPC() {
	// TODO Auto-generated constructor stub

}

ComWithPC::~ComWithPC() {
	// TODO Auto-generated destructor stub
}

int ComWithPC::Initialized(){return -1;}
void ComWithPC::Main(){}
int ComWithPC::SendData(uint8_t *data,uint32_t size){return 0;}
int ComWithPC::GetFrameFromBuffer(uint8_t *data){return 0;}
void ComWithPC::Reset(){};
void ComWithPC::TimeInterrupt(){};
int ComWithPC::addUartData(uint8_t *data,uint32_t size){return 0;}
