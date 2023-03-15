#include "gtest/gtest.h"
#include "Models/CircularBuffer.h"
TEST(CirculatBuffetTest, TestWorking) {
    constexpr int bufferSize=5;
    CircularBuffer<bufferSize,int> cbInt;
    for(int i=0;i<bufferSize;i++){
        ASSERT_TRUE(cbInt.addData(i));
    }
    for(int i=0;i<bufferSize;i++){
        ASSERT_FALSE(cbInt.addData(i));
    }
    for(int i=0;i<bufferSize;i++){
        int data;
        ASSERT_TRUE(cbInt.getData(&data));
        ASSERT_EQ(data,i);
    }
    for(int i=0;i<bufferSize/2;i++){
        ASSERT_TRUE(cbInt.addData(i));
    }
    for(int i=0;i<bufferSize/2;i++){
        int data;
        ASSERT_TRUE(cbInt.getData(&data));
        ASSERT_EQ(data,i);
    }
    for(int i=0;i<bufferSize;i++){
        ASSERT_TRUE(cbInt.addData(i));
    }
    for(int i=0;i<bufferSize;i++){
        int data;
        ASSERT_TRUE(cbInt.getData(&data));
        ASSERT_EQ(data,i);
    }
}