#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "VersionedTree.h"

namespace VersionedStructures {
    template <class Type> class Stack {
    public:
        Stack() { m_backPointer = m_tree.GetRoot(); }

        Type BackValue() {
            if (m_backPointer->IsEmpty()) {
                throw std::logic_error("Cannot get back value for empty stack!");
            }
            return m_backPointer->GetValue();
        }

        void Push(const Type &data) { m_backPointer = m_tree.AddNode(data, m_backPointer); }

        void Pop() { m_backPointer = m_tree.DeleteNode(m_backPointer); }

        [[nodiscard]] bool IsEmpty() const { return m_backPointer->IsEmpty(); }

        [[nodiscard]] std::vector<Type> ConvertToVector() const { return m_tree.ConvertToVector(m_backPointer); }

    private:
        Tree<Type> m_tree;
        std::shared_ptr<const INode<Type>> m_backPointer;
    };
}