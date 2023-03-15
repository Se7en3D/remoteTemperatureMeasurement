/*
 * main.cpp
 *
 *  Created on: 13 mar 2023
 *      Author: daniel
 */

#include<iostream>
#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

