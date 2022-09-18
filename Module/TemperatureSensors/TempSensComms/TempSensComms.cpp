/*
 * TempSensComms.cpp
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */

#include <stdio.h>
#include "TempSensComms.h"
TempSensComms::TempSensComms() {
	// TODO Auto-generated constructor stub

}

TempSensComms::~TempSensComms() {
	// TODO Auto-generated destructor stub
}
uint32_t TempSensComms::Reset(){
	return 240;
}
void TempSensComms::Write(uint8_t *data,uint32_t bitSize){

}
uint8_t TempSensComms::Read(){
	return 240;
}
