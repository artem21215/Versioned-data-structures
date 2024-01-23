#include <algorithm>
#include <vector>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedSet.h"

TEST(VersionedSetTests, VersionedSetSingleThread) {
    const size_t threadCount = 4;
    const std::vector<size_t> threadId = {0, 1, 2, 3};

    auto versionedSet = VersionedStructures::VersionedSet<int>(threadCount);
    EXPECT_NO_FATAL_FAILURE(versionedSet.Erase(threadId[1], 1));
    EXPECT_TRUE(versionedSet.IsEmpty(threadId[1]));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 1));
    EXPECT_FALSE(versionedSet.IsEmpty(threadId[1]));
    EXPECT_EQ(versionedSet.Size(threadId[1]), 1);
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 1));
    EXPECT_FALSE(versionedSet.Exists(threadId[1], 2));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 1));
    EXPECT_EQ(versionedSet.Size(threadId[1]), 1);
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 1));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 2));
    EXPECT_EQ(versionedSet.Size(threadId[1]), 2);
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 1));
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 2));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 7));
    EXPECT_EQ(versionedSet.Size(threadId[1]), 3);
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 1));
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 2));
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 7));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Erase(threadId[1], 7));
    EXPECT_EQ(versionedSet.Size(threadId[1]), 2);
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 1));
    EXPECT_TRUE(versionedSet.Exists(threadId[1], 2));
    EXPECT_FALSE(versionedSet.Exists(threadId[1], 7));
}