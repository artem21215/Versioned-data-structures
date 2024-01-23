#pragma once

#include <queue>

#include "VersionedStack.h"

namespace VersionedStructures {
    template <class Type> class Queue {
    public:
        Queue() { m_state = std::make_shared<StackState<Type>>(); }

        explicit Queue(const std::shared_ptr<StackState<Type>> &state) : m_state(state) {}

        Type BackValue() const {
            if (m_state->GetStartState()->IsEmpty()) {
                throw std::logic_error("Cannot get back value for empty queue!");
            }
            return m_state->GetStartState()->GetValue();
        }

        void Push(const Type &data, bool isRebase = false) {
            m_state->SetEndState(m_state->GetTree().AddNode(data, m_state->GetEndState()));
            if (!isRebase) {
                m_history.WriteEvent(HistoryAction::Insert, *m_state);
            }

            if (m_state->GetStartState()->IsEmpty()) {
                Pop(true);
            }
        }

        void Pop(bool isRebase = false) {
            if (m_state->GetStartState() == m_state->GetEndState()) {
                m_state->SetStartState(m_state->GetTree().GetRoot());
                m_state->SetEndState(m_state->GetTree().GetRoot());
                m_history.WriteEvent(HistoryAction::Erase, *m_state);
                return;
            }
            m_state->SetStartState(m_state->GetTree().FindChild(m_state->GetStartState(), m_state->GetEndState()));
            if (!isRebase) {
                m_history.WriteEvent(HistoryAction::Erase, *m_state);
            }
        }

        [[nodiscard]] bool IsEmpty() const { return m_state->GetEndState()->IsEmpty(); }

        [[nodiscard]] std::vector<Type> ConvertToVector() const {
            return m_state->GetTree().ConvertToVector(m_state->GetStartState(), m_state->GetEndState());
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