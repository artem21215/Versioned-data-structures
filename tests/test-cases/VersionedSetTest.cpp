#include <algorithm>
#include <vector>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedSet.h"

TEST(VersionedSetTests, VersionedSetSingleThread) {
    const size_t threadCount = 4;
    const std::vector<size_t> threadId = {0, 1, 2, 3};

    auto versionedSet = VersionedStructures::Set<int>(threadCount);
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

TEST(VersionedSetTests, VersionedSet) {
    const size_t threadCount = 4;
    const std::vector<size_t> threadId = {0, 1, 2, 3};

    auto versionedSet = VersionedStructures::Set<int>(threadCount);
    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[0], 1));
    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 1));
    EXPECT_EQ(versionedSet.Size(threadId[0]), 1);
    EXPECT_EQ(versionedSet.Size(threadId[1]), 1);

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[0], 2));
    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[1], 3));
    EXPECT_EQ(versionedSet.Size(threadId[0]), 2);
    EXPECT_EQ(versionedSet.Size(threadId[1]), 2);

    EXPECT_NO_FATAL_FAILURE(versionedSet.Erase(threadId[0], 1));
    EXPECT_EQ(versionedSet.Size(threadId[0]), 1);
    EXPECT_EQ(versionedSet.Size(threadId[1]), 2);

    EXPECT_NO_FATAL_FAILURE(versionedSet.Merge());
    EXPECT_EQ(versionedSet.Size(threadId[0]), 3);
    EXPECT_EQ(versionedSet.Size(threadId[1]), 3);
    EXPECT_EQ(versionedSet.Size(threadId[2]), 3);
    EXPECT_EQ(versionedSet.Size(threadId[3]), 3);

    EXPECT_NO_FATAL_FAILURE(versionedSet.Insert(threadId[0], 5));
    EXPECT_EQ(versionedSet.Size(threadId[0]), 4);
    EXPECT_EQ(versionedSet.Size(threadId[1]), 3);
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{1, 2, 3, 5}));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Undo(threadId[0]));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{1, 2, 3}));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Undo(threadId[0]));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{2}));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Undo(threadId[0]));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{1, 2}));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Undo(threadId[0]));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{1}));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[1]), ::testing::ElementsAreArray(std::vector{1, 2, 3}));

    EXPECT_NO_FATAL_FAILURE(versionedSet.Erase(threadId[1], 1));
    EXPECT_NO_FATAL_FAILURE(versionedSet.Merge());
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]), ::testing::ElementsAreArray(std::vector{1, 2, 3}));
    ASSERT_THAT(versionedSet.ConvertToVector(threadId[0]),
                ::testing::ElementsAreArray(versionedSet.ConvertToVector(threadId[1])));
}