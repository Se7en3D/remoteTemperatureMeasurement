/*
 * ContentTypeGetter.h
 *
 *  Created on: 18 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_CONTENTTYPEGETTER_H_
#define COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_CONTENTTYPEGETTER_H_
#include <string>
class ContentTypeGetter {
public:
	virtual ~ContentTypeGetter();
	virtual int getIntValue(const char *argument,const char* tag,const char *data, int *value)=0;
	virtual char* getCharValue(const char *argument,const char* tag,const char *data)=0;
	virtual int findStdString(const char *argument,const char* tag,const char *data, std::string &dataOut)=0;
};

#endif /* COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_CONTENTTYPEGETTER_H_ */
