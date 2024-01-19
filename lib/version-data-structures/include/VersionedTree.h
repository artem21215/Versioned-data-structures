#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace VersionedTree {
    template <class Type> class INode {
    public:
        INode() {}

        virtual void SetParent(const std::shared_ptr<const INode> &parent) = 0;

        [[nodiscard]] virtual std::shared_ptr<const INode> GetParent() const = 0;

        [[nodiscard]] virtual Type GetValue() const = 0;

        [[nodiscard]] virtual bool IsEmpty() const = 0;

    protected:
        std::shared_ptr<const INode<Type>> m_parent;
    };

    template <class Type> class EmptyNode : public INode<Type>, std::enable_shared_from_this<EmptyNode<Type>> {
    public:
        EmptyNode() : INode<Type>() {}

        void SetParent(const std::shared_ptr<const INode<Type>> &) override { this->m_parent = this->shared_from_this(); }

        [[nodiscard]] std::shared_ptr<const INode<Type>> GetParent() const override { return this->shared_from_this(); }

        [[nodiscard]] Type GetValue() const override { throw std::logic_error("Cannot return data from empty node!"); }

        [[nodiscard]] bool IsEmpty() const override { return true; }
    };

    template <class Type> class Node : public INode<Type> {
    public:
        explicit Node(const Type &data) : m_data(data) {}

        void SetParent(const std::shared_ptr<const INode<Type>> &parent) override {
            if (!parent) {
                throw std::logic_error(
                    "Parent must be not nullptr, for empty node you should use VersionedTree::EmptyNode class!");
            }
            this->m_parent = parent;
        }

        [[nodiscard]] std::shared_ptr<const INode<Type>> GetParent() const override {
            if (!this->m_parent) {
                throw std::logic_error("Node hasn't parrent, unknown case!");
            }
            return this->m_parent;
        }

        [[nodiscard]] Type GetValue() const override { return m_data; }

        [[nodiscard]] bool IsEmpty() const override { return false; }

    private:
        Type m_data;
    };

    template <class NodeType> class Tree {
    public:
        Tree() {
            std::shared_ptr<INode<NodeType>> emptyNode = std::make_shared<EmptyNode<NodeType>>();
            m_root = emptyNode;
        }

        std::shared_ptr<const INode<NodeType>> AddNode(const NodeType &nodeData,
                                                       const std::shared_ptr<const INode<NodeType>> &parrentNode) {
            auto newNode = Node<NodeType>(nodeData);
            try {
                newNode.SetParent(parrentNode);
            } catch (const std::logic_error &e) {
                std::cout << e.what() << ' ' << "So node was added to root!" << std::endl;
                newNode.SetParent(GetRoot());
            }
            return std::make_shared<const Node<NodeType>>(newNode);
        }

        std::shared_ptr<const INode<NodeType>> DeleteNode(std::shared_ptr<const INode<NodeType>> &nodeToDelete) {
            if (!nodeToDelete) {
                std::cout << "Node for deletting is nullptr! Nothing was done!" << std::endl;
                return nodeToDelete->GetParent();
            }

            return nodeToDelete->GetParent();
        }

        std::shared_ptr<const INode<NodeType>> GetRoot() const { return m_root; }

        std::vector<NodeType> ConvertToVector(const std::shared_ptr<const INode<NodeType>> &finishNode) {
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