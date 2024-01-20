#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace VersionedStructures {
    template <class Type> class INode : public std::enable_shared_from_this<INode<Type>> {
    public:
        virtual void SetParent(const std::shared_ptr<const INode> &parent) = 0;

        [[nodiscard]] virtual std::shared_ptr<const INode> GetParent() const = 0;

        [[nodiscard]] virtual Type GetValue() const = 0;

        [[nodiscard]] virtual bool IsEmpty() const = 0;
    };

    template <class Type> class EmptyNode : public INode<Type> {
    public:
        void SetParent(const std::shared_ptr<const INode<Type>> &) override {}

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
        std::shared_ptr<const INode<Type>> m_parent;
    };
}