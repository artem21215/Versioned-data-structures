#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedQueue.h"

TEST(VersionedQueueTests, EmptyTree) {
    auto queue = VersionedStructures::Queue<int>();
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_TRUE(queue.IsEmpty());

    EXPECT_NO_FATAL_FAILURE(queue.Push(5));
    EXPECT_EQ(queue.BackValue(), 5);
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_TRUE(queue.IsEmpty());

    EXPECT_NO_FATAL_FAILURE(queue.Push(3));
    EXPECT_NO_FATAL_FAILURE(queue.Push(3));
    EXPECT_EQ(queue.BackValue(), 3);
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_EQ(queue.BackValue(), 3);
    EXPECT_NO_FATAL_FAILURE(queue.Push(5));
    EXPECT_NO_FATAL_FAILURE(queue.Push(2));
    EXPECT_NO_FATAL_FAILURE(queue.Push(1));
    EXPECT_EQ(queue.BackValue(), 3);
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_NO_FATAL_FAILURE(queue.Pop());
    EXPECT_EQ(queue.BackValue(), 2);
    EXPECT_FALSE(queue.IsEmpty());

    const auto actualConverted = queue.ConvertToVector();
    const std::vector<int> expectedConverted{2, 1};
    EXPECT_EQ(actualConverted.size(), expectedConverted.size());
    ASSERT_THAT(actualConverted, ::testing::ElementsAreArray(expectedConverted));
}