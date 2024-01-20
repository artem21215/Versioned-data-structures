#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "VersionedTreeNode.h"

TEST(VersionedNodeTests, NodeOperations) {
    const std::string goodBye = "GoodBye2023";
    const std::string hello = "Hello2024";

    const auto root = std::make_shared<VersionedStructures::Node<std::string>>(goodBye);
    EXPECT_EQ(root->GetValue(), goodBye);

    VersionedStructures::Node<std::string> nextNode{hello};
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