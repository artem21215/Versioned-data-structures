#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedTree.h"

TEST(VersionedTreeTests, testConvert) {
    auto tree = VersionedTree::Tree<int>{};
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
    auto tree = VersionedTree::Tree<float>{};
    EXPECT_NO_FATAL_FAILURE(tree.ConvertToVector(nullptr));
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

TEST(VersionedNodeTests, NodeOperations) {
    const std::string goodBye = "GoodBye2023";
    const std::string hello = "Hello2024";

    const auto root = std::make_shared<VersionedTree::Node<std::string>>(goodBye);
    EXPECT_EQ(root->GetValue(), goodBye);

    VersionedTree::Node<std::string> nextNode{hello};
    nextNode.SetParent(root);
    EXPECT_EQ(nextNode.GetValue(), hello);
    const auto parent = nextNode.GetParent();
    ASSERT_TRUE(parent);
    EXPECT_EQ(parent->GetValue(), goodBye);
    try {
        const auto parentOfRoot = parent->GetParent();
        FAIL();
    } catch (const std::logic_error &e) {
        ASSERT_EQ(e.what(), std::string("Node hasn't parrent, unknown case!"));
    }

    try {
        nextNode.SetParent(nullptr);
        FAIL();
    } catch (const std::logic_error &e) {
        ASSERT_EQ(
            e.what(),
            std::string("Parent must be not nullptr, for empty node you should use VersionedTree::EmptyNode class!"));
    }
}