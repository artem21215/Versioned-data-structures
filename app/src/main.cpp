#include <iostream>

#include "VersionedStack.h"
#include "VersionedTree.h"
#include "VersionedTreeNode.h"

int main() {
    auto stack = VersionedStructures::Stack<int>();
    stack.Pop();
    stack.Push(5);
    stack.Push(5);
    stack.Push(8);
    stack.Push(3);
    stack.Pop();
    const auto convertedStack = stack.ConvertToVector();
    for (const auto &elem : convertedStack) {
        std::cout << elem << std::endl;
    }

    return 0;
}
