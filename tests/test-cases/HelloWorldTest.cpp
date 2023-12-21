#include <gtest/gtest.h>

#include "HelloWorld.h"

TEST(HelloWorldBaseTests, testEqual){
    ASSERT_EQ(TemporarySpace::getHelloWorldString(), "Hello my World!");
}