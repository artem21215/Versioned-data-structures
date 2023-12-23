#include <gtest/gtest.h>

#include "HelloWorld.h"

TEST(HelloWorldBaseTests, testEqual){
    ASSERT_EQ(TemporarySpace::getHelloWorldString(), "Hello my World!");
}

TEST(HelloWorldBaseTests, testEqual2){
    EXPECT_EQ(TemporarySpace::getHelloWorldString(), "Hello my World!");
}

TEST(HelloWorldBaseTests, testEqual3){
    ASSERT_EQ(0,0);
}