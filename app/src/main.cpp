#include <future>
#include <iostream>
#include <thread>

#include "VersionedStack.h"
#include "VersionedTree.h"
#include "VersionedTreeNode.h"

struct VersionInfo {
    std::shared_ptr<const VersionedStructures::INode<int>> commonVersion;
    std::shared_ptr<const VersionedStructures::INode<int>> lastThreadVersion;
};

void AddElements(VersionedStructures::Stack<int> stack, std::promise<VersionInfo> treeUpdatePromise) {
    VersionInfo threadVersionInfo;
    threadVersionInfo.commonVersion = stack.GetCurrentVersion();
    stack.Push(12);
    stack.Push(13);
    stack.Push(14);
    threadVersionInfo.lastThreadVersion = stack.GetCurrentVersion();
    treeUpdatePromise.set_value(threadVersionInfo);
}

std::shared_ptr<const VersionedStructures::INode<int>>
Merge(std::thread &thread, std::future<VersionInfo> &future,
      const std::shared_ptr<const VersionedStructures::INode<int>> &rebaseTarget,
      VersionedStructures::Tree<int> &tree) {
    thread.join();
    auto threadLastVersion = future.get();
    const auto convertedStack = tree.ConvertToVector(threadLastVersion.lastThreadVersion);
    for (const auto &elem : convertedStack) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    auto current = threadLastVersion.lastThreadVersion;
    auto result = rebaseTarget;
    while (current != threadLastVersion.commonVersion) {
        result = tree.AddNode(current->GetValue(), result);
        current = current->GetParent();
    }
    return result;
}

int main() {
    auto stack = VersionedStructures::Stack<int>();
    stack.Pop();
    stack.Push(5);
    stack.Push(5);
    stack.Push(8);
    stack.Push(3);
    stack.Pop();

    std::promise<VersionInfo> treeUpdatePromise;
    std::future<VersionInfo> treeUpdateFuture = treeUpdatePromise.get_future();
    std::thread t(AddElements, stack, std::move(treeUpdatePromise));

    stack.Push(0);
    stack.Push(0);
    stack.Push(0);
    stack.Push(0);

    auto &tree = stack.GetTree();
    const auto rebasedVersion = Merge(t, treeUpdateFuture, stack.GetCurrentVersion(), tree);

    const auto convertedStack = stack.ConvertToVector(rebasedVersion);
    for (const auto &elem : convertedStack) {
        std::cout << elem << ' ';
    }

    return 0;
}
