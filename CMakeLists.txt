cmake_minimum_required(VERSION 3.14)

project(tcpTempSens_gt)
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)


set(EXEC_FILES 
    Test/main.cpp
    Test/ModuleTest/Models/CircularBufferTest.cpp
    Test/ModuleTest/CommunicationWithPC/ESP8266StateMachine/ESP8266StateMachineTest.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266CheckCIPSTATUS.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToRouter.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266ConnectToTCPServer.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266SetMode.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266Initialized.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266State.cpp
    Module/CommunicationWithPC/ESP8266StateMachine/ESP8266SetPassthroughMode.cpp
)
set(INC_DIRECT 
  Module/
  Test/include/
)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(${PROJECT_NAME}
                ${EXEC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRECT})
target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})