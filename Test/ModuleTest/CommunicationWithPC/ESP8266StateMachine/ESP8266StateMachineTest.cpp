#include "include/gtest/gtest.h"
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.h"
#include "CommunicationWithPC/ESP8266StateMachine/ESP8266Definitions.h"

TEST(ESP8266StateTest,ESP8266CheckCIPSTATUSNextStates){
    ESP8266CheckCIPSTATUS esp8266CheckCIPSTATUS{};
    std::string stringStatus0{"OK\r\n STATUS:0"};
    std::string stringStatus1{"OK\r\n STATUS:1"};
    std::string stringStatus2{"OK\r\n STATUS:2"};
    std::string stringStatus3{"OK\r\n STATUS:3"};
    std::string stringStatus4{"OK\r\n STATUS:4"};
    std::string stringStatus5{"OK\r\n STATUS:5"};
    std::string stringStatus6{"OK\r\n STATUS:6"};
    std::string stringwithEmptyStatus{"OK\r\n"};
    std::string stringwithEmptyData{"EASDFASD\r\n"};
    ESP8266State* nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus0);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::checkCIPSTATUS);
    delete nextState;

    for(int i=0;i<50;i++){
        nextState=esp8266CheckCIPSTATUS.getNextState(stringwithEmptyData);
        ASSERT_EQ(nextState,nullptr);
    }

    nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus1);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::checkCIPSTATUS);
    delete nextState;
    
    nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus2);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::connectToTCPServer);
    delete nextState;

    nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus3);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::initialized);
    delete nextState;

    nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus4);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::connectToTCPServer);
    delete nextState;

    nextState=esp8266CheckCIPSTATUS.getNextState(stringStatus6);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::checkCIPSTATUS);
    delete nextState;

    nextState=esp8266CheckCIPSTATUS.getNextState(stringwithEmptyStatus);
    ASSERT_NE(nextState,nullptr);
    ASSERT_EQ(nextState->getStateName(),ESP8266::checkCIPSTATUS);
    delete nextState;
}