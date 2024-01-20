#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedStack.h"

TEST(VersionedStackTests, EmptyTree) {
    auto stack = VersionedStructures::Stack<int>();
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_TRUE(stack.IsEmpty());

    EXPECT_NO_FATAL_FAILURE(stack.Push(5));
    EXPECT_EQ(stack.BackValue(), 5);
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_TRUE(stack.IsEmpty());

    EXPECT_NO_FATAL_FAILURE(stack.Push(3));
    EXPECT_NO_FATAL_FAILURE(stack.Push(3));
    EXPECT_EQ(stack.BackValue(), 3);
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_EQ(stack.BackValue(), 3);
    EXPECT_NO_FATAL_FAILURE(stack.Push(5));
    EXPECT_NO_FATAL_FAILURE(stack.Push(2));
    EXPECT_NO_FATAL_FAILURE(stack.Push(1));
    EXPECT_EQ(stack.BackValue(), 1);
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_NO_FATAL_FAILURE(stack.Pop());
    EXPECT_EQ(stack.BackValue(), 5);
    EXPECT_FALSE(stack.IsEmpty());

    const auto actualConverted = stack.ConvertToVector();
    const std::vector<int> expectedConverted{3, 5};
    EXPECT_EQ(actualConverted.size(), expectedConverted.size());
    ASSERT_THAT(actualConverted, ::testing::ElementsAreArray(expectedConverted));
}