#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "VersionedTreeNode.h"

namespace VersionedStructures {
    template <class NodeType> class Tree {
    public:
        Tree() : m_root(std::make_shared<EmptyNode<NodeType>>()) {}

        [[nodiscard]] std::shared_ptr<const INode<NodeType>>
        AddNode(const NodeType &nodeData, const std::shared_ptr<const INode<NodeType>> &parrentNode) const {
            auto newNode = Node<NodeType>(nodeData);
            try {
                newNode.SetParent(parrentNode);
            } catch (const std::logic_error &e) {
                std::cout << e.what() << ' ' << "So node was added to root!" << std::endl;
                newNode.SetParent(GetRoot());
            }
            return std::make_shared<const Node<NodeType>>(newNode);
        }

        [[nodiscard]] std::shared_ptr<const INode<NodeType>>
        DeleteNode(const std::shared_ptr<const INode<NodeType>> &nodeToDelete) const {
            if (!nodeToDelete) {
                std::cout << "Node for deletting is nullptr! Nothing was done!" << std::endl;
                return nodeToDelete->GetParent();
            }

            return nodeToDelete->GetParent();
        }

        [[nodiscard]] std::shared_ptr<const INode<NodeType>> GetRoot() const { return m_root; }

        [[nodiscard]] std::vector<NodeType>
        ConvertToVector(const std::shared_ptr<const INode<NodeType>> &finishNode) const {
            std::vector<NodeType> result;
            if (!finishNode) {
                return result;
            }

            auto current = finishNode;
            while (!current->IsEmpty()) {
                result.push_back(current->GetValue());
                current = current->GetParent();
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

    private:
        std::shared_ptr<INode<NodeType>> m_root;
    };
}