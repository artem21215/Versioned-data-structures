#include <algorithm>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "HashSet.h"

TEST(VersionedStackTests, HashSet) {
    auto hashSet = VersionedStructures::HashSet<int>();
    EXPECT_NO_FATAL_FAILURE(hashSet.Erase(1));
    EXPECT_TRUE(hashSet.IsEmpty());

    EXPECT_NO_FATAL_FAILURE(hashSet.Insert(1));
    EXPECT_FALSE(hashSet.IsEmpty());
    EXPECT_EQ(hashSet.Size(), 1);
    EXPECT_TRUE(hashSet.Exists(1));
    EXPECT_FALSE(hashSet.Exists(2));

    EXPECT_NO_FATAL_FAILURE(hashSet.Insert(1));
    EXPECT_EQ(hashSet.Size(), 1);
    EXPECT_TRUE(hashSet.Exists(1));

    EXPECT_NO_FATAL_FAILURE(hashSet.Insert(2));
    EXPECT_EQ(hashSet.Size(), 2);
    EXPECT_TRUE(hashSet.Exists(1));
    EXPECT_TRUE(hashSet.Exists(2));

    EXPECT_NO_FATAL_FAILURE(hashSet.Insert(7));
    EXPECT_EQ(hashSet.Size(), 3);
    EXPECT_TRUE(hashSet.Exists(1));
    EXPECT_TRUE(hashSet.Exists(2));
    EXPECT_TRUE(hashSet.Exists(7));

    EXPECT_NO_FATAL_FAILURE(hashSet.Erase(7));
    EXPECT_EQ(hashSet.Size(), 2);
    EXPECT_TRUE(hashSet.Exists(1));
    EXPECT_TRUE(hashSet.Exists(2));
    EXPECT_FALSE(hashSet.Exists(7));

    std::vector<int> expectedConverted;

    for (int i = 1; i < 100; i++) {
        expectedConverted.push_back(i);
        hashSet.Insert(i);
    }

    const auto actualConverted = hashSet.ConvertToVector();

    std::sort(expectedConverted.begin(), expectedConverted.end(),
              [hashSet](const auto &lhs, const auto &rhs) {
                  const size_t lhsHash = std::hash<int>{}(lhs) % hashSet.BucketSize();
                  const size_t rhsHash = std::hash<int>{}(rhs) % hashSet.BucketSize();
                  return lhsHash < rhsHash;
              });

    EXPECT_EQ(actualConverted.size(), expectedConverted.size());
    ASSERT_THAT(actualConverted, ::testing::ElementsAreArray(expectedConverted));
}