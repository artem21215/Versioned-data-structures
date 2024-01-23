#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <iostream>

#include "StackHistory.h"

TEST(StackHistoryTests, HistoryOperations) {
    VersionedStructures::History<int> history;
    EXPECT_NO_FATAL_FAILURE(history.Dump(std::cout));
    history.WriteEvent(VersionedStructures::HistoryAction::Insert, VersionedStructures::StackState<int>{});
    history.WriteEvent(VersionedStructures::HistoryAction::Erase, VersionedStructures::StackState<int>{});
    history.WriteEvent(VersionedStructures::HistoryAction::Merge, VersionedStructures::StackState<int>{});
    EXPECT_NO_FATAL_FAILURE(history.Pop());
    EXPECT_TRUE(history.GetLastVersion()->GetStartState());
    EXPECT_TRUE(history.GetLastVersion()->GetEndState());
}