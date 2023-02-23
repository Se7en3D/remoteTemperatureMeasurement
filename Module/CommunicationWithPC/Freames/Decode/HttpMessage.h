#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
#include <cstdint>
#include <string>
#include <vector>
enum class httpMessageMethod{
	NONE,
    GET,
    POST,
    HEAD,
    OPTIONS,
    PUT,
    DELETE,
    CONNECTION,
    TRACE
};
enum class httpMessageType{
	NONE,
	REQUEST,
	ANSWER
};
struct HttpFrame{
    httpMessageMethod method;
    httpMessageType type;
    int answerCode;
    std::vector<std::string> header;
    std::string body;
    std::string startLine;
};
class HttpMessage
{
#ifdef TESTMODULE
	friend class HttpMessageTest;
#endif
public:
    HttpMessage();
    void addData(std::string &data);
    bool isMessageComplete();
    int getFrame(HttpFrame *frame);
    void clearData();
private:
    enum StateOfCompleteData{
    	WaitingForStartLine,
		WaitingForEmptyLine,
		WaitingForLastBodyByte,
		FrameComplete
    };
    StateOfCompleteData stateOfCompleteData;
    httpMessageMethod method;
    httpMessageType type;
    int answerCode;
    size_t bodyLength=0;
    std::string tempBuffer;
    std::vector<std::string> header;
    std::string body;
    std::string startLine;
    std::vector<std::string> stringSplit(const std::string &data,const std::string &del,bool includeEmptyLine=false);
    int findStartLine();
    int findEmptyLine();
    int waitToGetLastByteOfBody();
    void findContentLength();
    int findAnswerCode();
};

#endif // HTTPMESSAGE_H
