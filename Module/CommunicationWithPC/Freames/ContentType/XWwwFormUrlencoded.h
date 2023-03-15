/*
 * XWwwFormUrlencoded.h
 *
 *  Created on: 18 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_XWWWFORMURLENCODED_H_
#define COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_XWWWFORMURLENCODED_H_
#include "ContentTypeGetter.h"
class XWwwFormUrlencoded: public ContentTypeGetter{
public:
	XWwwFormUrlencoded();
	virtual ~XWwwFormUrlencoded();
	int getIntValue(const char *argument,const char* tag,const char *data, int *value) override ;
	char* getCharValue(const char *argument,const char* tag,const char *data) override;
	int findStdString(const char *argument,const char* tag,const char *data, std::string &dataOut) override;
};


#endif /* COMMUNICATIONWITHPC_FREAMES_CONTENTTYPE_XWWWFORMURLENCODED_H_ */
