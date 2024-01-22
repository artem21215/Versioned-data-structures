#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "VersionedTree.h"

namespace VersionedStructures {
    template <class Type> class StackState {
    public:
        StackState() {
            m_startStackPointer = m_tree.GetRoot();
            m_backStackPointer = m_tree.GetRoot();
        }

        void SetStartState(const std::shared_ptr<const INode<Type>> &node) { m_startStackPointer = node; }

        void SetEndState(const std::shared_ptr<const INode<Type>> &node) { m_backStackPointer = node; }

        std::shared_ptr<const INode<Type>> GetStartState() const { return m_startStackPointer; }

        std::shared_ptr<const INode<Type>> GetEndState() const { return m_backStackPointer; }

        Tree<Type> GetTree() const { return m_tree; }

    private:
        Tree<Type> m_tree;
        std::shared_ptr<const INode<Type>> m_startStackPointer;
        std::shared_ptr<const INode<Type>> m_backStackPointer;
    };
}