//
// Created by artem on 08.01.2023.
//

#include "lib/HelloWorld.h"

#include <gtest/gtest.h>

TEST(HelloWorldBaseTests, testEqual){
    ASSERT_EQ(TemporarySpace::getHelloWorldString(), "HelloWorld!");
}