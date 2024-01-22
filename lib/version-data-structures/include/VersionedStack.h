#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "StackHistory.h"
#include "VersionedTree.h"

namespace VersionedStructures {
    template <class Type> class Stack {
    public:
        Stack() {}

        Stack(const std::shared_ptr<StackState<Type>> &state) : m_state(state) {}

        Type BackValue() const {
            if (m_state->GetEndState()->IsEmpty()) {
                throw std::logic_error("Cannot get back value for empty stack!");
            }
            return m_state->GetEndState()->GetValue();
        }

        void Push(const Type &data, bool isRebase = false) {
            m_state->SetEndState(m_state->GetTree().AddNode(data, m_state->GetEndState()));
            if (!isRebase) {
                m_history.WriteEvent(HistoryAction::Insert, *m_state);
            }
        }

        void Pop() {
            m_state->SetEndState(m_state->GetTree().DeleteNode(m_state->GetEndState()));
            m_history.WriteEvent(HistoryAction::Erase, *m_state);
        }

        [[nodiscard]] bool IsEmpty() const { return m_state->GetEndState()->IsEmpty(); }

        [[nodiscard]] std::vector<Type> ConvertToVector() const {
            return m_state->GetTree().ConvertToVector(m_state->GetEndState());
        }

        [[nodiscard]] std::vector<Type> ConvertToVector(const std::shared_ptr<const INode<Type>> &version) const {
            return m_state->GetTree().ConvertToVector(version);
        }

        void Rebase(const StackState<Type> &stackState) {
            std::stack<Type> rebaseData;
            auto currentNode = stackState.GetEndState();
            while (currentNode != stackState.GetStartState()) {
                rebaseData.push(currentNode->GetValue());
                currentNode = currentNode->GetParent();
            }
            while (!rebaseData.empty()) {
                Push(rebaseData.top(), true);
                rebaseData.pop();
            }
            m_history.WriteEvent(HistoryAction::Merge, *m_state);
        }

        void Undo() {
            m_history.Pop();
            m_state = m_history.GetLastVersion();
        }

        [[nodiscard]] History<Type> GetHistory() const { return m_history; }

    private:
        std::shared_ptr<StackState<Type>> m_state;
        History<Type> m_history;
    };
}