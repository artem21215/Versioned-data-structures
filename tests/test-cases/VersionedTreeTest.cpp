#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedTree.h"

TEST(VersionedTreeTests, testConvert) {
    auto tree = VersionedStructures::Tree<int>{};
    const auto someNodes = tree.AddNode(6, tree.AddNode(6, tree.AddNode(5, tree.GetRoot())));
    auto nodeToDelete = tree.AddNode(8, someNodes);
    const auto nodeAfterDelete = tree.DeleteNode(nodeToDelete);

    const auto actualWithOldNode = tree.ConvertToVector(nodeToDelete);
    const std::vector<int> expectedForOldNode{5, 6, 6, 8};
    ASSERT_EQ(actualWithOldNode.size(), expectedForOldNode.size());
    ASSERT_THAT(actualWithOldNode, ::testing::ElementsAreArray(expectedForOldNode));

    const auto actualWithAfterDeleteNode = tree.ConvertToVector(nodeAfterDelete);
    const std::vector<int> expectedForAfterDeleteNode{5, 6, 6};
    ASSERT_EQ(actualWithAfterDeleteNode.size(), expectedForAfterDeleteNode.size());
    ASSERT_THAT(actualWithAfterDeleteNode, ::testing::ElementsAreArray(expectedForAfterDeleteNode));
}

TEST(VersionedTreeTests, EmptyTree) {
    auto tree = VersionedStructures::Tree<float>{};
    EXPECT_NO_FATAL_FAILURE(const auto vec = tree.ConvertToVector(nullptr));
    const auto rootFirst = tree.GetRoot();
    EXPECT_TRUE(rootFirst);
    EXPECT_TRUE(rootFirst->IsEmpty());
    auto node = tree.AddNode(5.5, rootFirst);
    EXPECT_FALSE(node->IsEmpty());
    EXPECT_NEAR(node->GetValue(), 5.5, 1e-10);
    const auto afterDelete = tree.DeleteNode(node);
    EXPECT_TRUE(afterDelete);
    EXPECT_TRUE(afterDelete->IsEmpty());
    try {
        [[maybe_unused]] const auto value = afterDelete->GetValue();
        FAIL();
    } catch (const std::logic_error &e) {
        EXPECT_EQ(e.what(), std::string("Cannot return data from empty node!"));
    }
    EXPECT_TRUE(node);
    EXPECT_FALSE(node->IsEmpty());
    EXPECT_NEAR(node->GetValue(), 5.5, 1e-10);
}