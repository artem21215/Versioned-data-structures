#include <iostream>

#include "VersionedTree.h"
#include "VersionedTreeNode.h"

int main() {
    auto tree = VersionedStructures::Tree<int>{};
    const auto firstNode = tree.AddNode(5, tree.GetRoot());
    const auto secondNode = tree.AddNode(6, firstNode);
    auto thirdNode = tree.AddNode(6, secondNode);
    const auto fourthNode = tree.AddNode(8, thirdNode);
    const auto afterDelete = tree.DeleteNode(thirdNode);

    const auto curSequence = tree.ConvertToVector(afterDelete);
    for (const auto &elem : curSequence) {
        std::cout << elem << std::endl;
    }
    return 0;
}
